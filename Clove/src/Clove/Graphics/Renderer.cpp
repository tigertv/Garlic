#include "Renderer.hpp"

#include "Clove/Graphics/RenderCommand.hpp"
#include "Clove/Graphics/Bindables/VertexBuffer.hpp"
#include "Clove/Graphics/Bindables/IndexBuffer.hpp"
#include "Clove/Graphics/BindableFactory.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"
#include "Clove/Graphics/Bindables/Texture.hpp"
#include "Clove/Graphics/RenderTarget.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Material.hpp"
#include "Clove/Graphics/Mesh.hpp"
#include "Clove/Graphics/Sprite.hpp"
#include "Clove/Application.hpp"
#include "Clove/Platform/Window.hpp"
#include "Clove/Graphics/Bindables/Shader.hpp"

namespace clv::gfx{
	//struct SceneData{
	//
	//	-camera
	//	-lights
	//	-mesh
	//
	// etc
	//
	//} *currentSceneData;

	//Camera
	std::shared_ptr<gfx::ShaderBufferObject<ViewData>> Renderer::viewDataSBO; //Matrices for vertex shader
	std::shared_ptr<gfx::ShaderBufferObject<ViewPos>> Renderer::viewPosSBO; //position for pixel shader

	//Lights
	std::shared_ptr<gfx::ShaderBufferObject<PointLightShaderData>> Renderer::lightDataSBO; //Data about each light
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowDepthData>> Renderer::shadowDepthData; //Data about each light's pos / farplane specifically for the lit ps
	std::shared_ptr<gfx::ShaderBufferObject<LightNumAlignment>> Renderer::lightNumSBO; //Total number of lights

	//Shadows
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowShaderData>> Renderer::shadowDataSBO; //Binds in each individual transform from the light for the geometry shader
	std::shared_ptr<gfx::ShaderBufferObject<PointShadowData>> Renderer::currentDepthData; //for shadow map generation (a specific light for the gs)
	std::shared_ptr<gfx::ShaderBufferObject<LightNumAlignment>> Renderer::faceIndexStartSBO; //Which face the Geometry shader starts on (when sending data to the pixel shader)
	
	std::shared_ptr<gfx::Shader> Renderer::cubeShadowMapShader; //Shadow for generating the shadow map

	//Mesh
	std::vector<std::shared_ptr<Mesh>> Renderer::meshesToRender;

	PointLightShaderData Renderer::currentLightInfo; //Collects the current light info to bind in
	PointShadowDepthData Renderer::currentShadowDepth; //Collects the current depth info to bind in
	uint32 Renderer::numLights;

	std::array<std::array<math::Matrix4f, 6>, MAX_LIGHTS> Renderer::shadowTransforms = {}; //Each light's shadow transform

	std::shared_ptr<RenderTarget> Renderer::shadowMapRenderTarget; //Render target for the shadow map
	std::shared_ptr<Texture> Renderer::shadowMapTexture; //Texture for the shadow map
	
	std::shared_ptr<RenderTarget> Renderer::customRenderTarget;

	void Renderer::initialise(){
		CLV_LOG_TRACE("Initialising renderer");

		//currentSceneData = new SceneData();

		viewDataSBO = BindableFactory::createShaderBufferObject<ViewData>(ShaderType::Vertex, BBP_CameraMatrices);
		viewPosSBO = BindableFactory::createShaderBufferObject<ViewPos>(ShaderType::Pixel, BBP_ViewData);
		
		lightDataSBO = BindableFactory::createShaderBufferObject<PointLightShaderData>(ShaderType::Pixel, BBP_PointLightData);

		shadowDataSBO = BindableFactory::createShaderBufferObject<PointShadowShaderData>(ShaderType::Geometry, BBP_ShadowData);

		shadowDepthData = BindableFactory::createShaderBufferObject<PointShadowDepthData>(ShaderType::Pixel, BBP_CubeDepthData);
		currentDepthData = BindableFactory::createShaderBufferObject<PointShadowData>(ShaderType::Pixel, BBP_CurrentDepthData);

		cubeShadowMapShader = BindableFactory::createShader(gfx::ShaderStyle::CubeShadowMap);
		lightNumSBO = BindableFactory::createShaderBufferObject<LightNumAlignment>(ShaderType::Pixel, BBP_CurrentLights);
		faceIndexStartSBO = BindableFactory::createShaderBufferObject<LightNumAlignment>(ShaderType::Geometry, BBP_CurrentFaceIndex);

		viewDataSBO->bind();
		viewPosSBO->bind();

		lightDataSBO->bind();
		shadowDataSBO->bind();
		shadowDepthData->bind();
		lightNumSBO->bind();
		faceIndexStartSBO->bind();

		shadowMapTexture = BindableFactory::createTexture(TBP_Shadow, { TextureStyle::Cubemap, TextureUsage::RenderTarget_Depth, { shadowMapSize, shadowMapSize }, MAX_LIGHTS });
		shadowMapRenderTarget = RenderTarget::createRenderTarget(nullptr, shadowMapTexture.get());

		RenderCommand::setBlendState(true);
	}

	void Renderer::shutDown(){
		CLV_LOG_TRACE("Shutting down renderer");

		//delete currentSceneData;
	}

	void Renderer::beginScene(){
		numLights = 0;
	}

	void Renderer::endScene(){
		lightDataSBO->update(currentLightInfo);
		shadowDepthData->update(currentShadowDepth);
		lightNumSBO->update({ numLights });

		const auto draw = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		//TODO: anyway to use above lambda?
		const auto drawShadow = [](const std::shared_ptr<Mesh>& mesh){
			mesh->bind();
			cubeShadowMapShader->bind(); //Will DX like this?
			RenderCommand::drawIndexed(mesh->getIndexCount());
		};

		RenderCommand::setDepthBuffer(true);

		//Calculate shadow map
		for(uint32 i = 0; i < numLights; i++){ //TODO: Remove raw loop
			shadowDataSBO->bind();
			shadowDataSBO->update({ shadowTransforms[i] });
			currentDepthData->bind();
			currentDepthData->update({ currentShadowDepth.depths[i] });

			faceIndexStartSBO->update({ i * 6 });

			RenderCommand::setViewPortSize(shadowMapSize, shadowMapSize);
			RenderCommand::setRenderTarget(*shadowMapRenderTarget); //TODO: How to set view level????
			std::for_each(meshesToRender.begin(), meshesToRender.end(), drawShadow);
			RenderCommand::setViewPortSize(Application::get().getWindow().getWidth(), Application::get().getWindow().getHeight());
		}
		
		RenderCommand::removeCurrentGeometryShader();

		RenderCommand::resetRenderTarget(); //Reset render target before binding the shadow map

		shadowMapTexture->bind(); //Bind this in before rendering the real mesh

		//Render any other render targets
		if(customRenderTarget){
			RenderCommand::setRenderTarget(*customRenderTarget);
			std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);
			RenderCommand::resetRenderTarget();
		}

		//Render scene
		std::for_each(meshesToRender.begin(), meshesToRender.end(), draw);

		RenderCommand::removeTextureAtSlot(TBP_Shadow);

		meshesToRender.clear();
	}

	void Renderer::setRenderTarget(const std::shared_ptr<RenderTarget>& inRenderTarget){
		customRenderTarget = inRenderTarget;
	}

	void Renderer::clearRenderTargets(){
		RenderCommand::setRenderTarget(*shadowMapRenderTarget);
		RenderCommand::clear(); //TODO: Might need to just do a clear depth command
		if(customRenderTarget){
			RenderCommand::setRenderTarget(*customRenderTarget);
			RenderCommand::clear();
		}
		RenderCommand::resetRenderTarget();
		RenderCommand::clear();
	}

	void Renderer::removeRenderTarget(){
		customRenderTarget.reset();
		RenderCommand::resetRenderTarget();
	}

	void Renderer::submitMesh(const std::shared_ptr<Mesh>& mesh){
		meshesToRender.push_back(mesh);
	}

	void Renderer::setCamera(const CameraRenderData& data){
		viewDataSBO->update({ data.lookAt, data.projection });
		viewPosSBO->update({ data.position });
	}

	void Renderer::submitPointLight(const PointLightData& data){
		const int32 lightIndex = numLights++;
		currentLightInfo.intensities[lightIndex] = data.intensity;

		shadowTransforms[lightIndex] = data.shadowTransforms;

		currentShadowDepth.depths[lightIndex].farPlane = data.farPlane;
		currentShadowDepth.depths[lightIndex].lightPos = data.intensity.position;
	}
}