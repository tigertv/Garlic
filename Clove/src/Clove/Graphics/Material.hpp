#pragma once

//#include "Clove/Graphics/MaterialData.hpp"
#include "Clove/Graphics/Bindables/ShaderBufferObject.hpp"
#include "Clove/Graphics/BindableFactory.hpp"

namespace clv::gfx{
	class Shader;
	class Texture;

	class Material{
		//VARIABLES
	private:
		math::Matrix4f modelData{}; //This should be an sbo

		//Not quite sure how this will hold the shader. Need to look at perhaps refactoring how the vertex layout is done for dx to remove the dependancy 
		//std::shared_ptr<Shader> shader;
		std::shared_ptr<Texture> albedoTexture;
		std::shared_ptr<Texture> specTexture;
		std::unordered_map<BufferBindingPoint, std::shared_ptr<Bindable>> shaderData;

		/*
		holds data about a mesh looks (and maybe sprites?)
		-holds the textures
		-holds the sbos (or the BufferData class/struct that will replace)
		-I think it should hold the shader but that might be difficult
		*/


		//FUNCTIONS
	public:
		Material();
		Material(const Material& other);
		Material& operator=(const Material& other);
		Material(Material&& other) noexcept;
		Material& operator=(Material&& other) noexcept;
		~Material();

		void bind(); //this'll bind the shader and textures

		//TODO: inl
		template<typename T>
		void setData(BufferBindingPoint bindingPoint, T&& data){
			if(auto iter = shaderData.find(bindingPoint); iter != shaderData.end()){
				if(auto sbo = std::dynamic_pointer_cast<ShaderBufferObject<T>>(iter->second)){
					sbo->update(data);
					return;
				}
			}
			/*
			TODO:
			The below needs the shader type! :(
			*/
			shaderData[bindingPoint] = BindableFactory::createShaderBufferObject<T>(bindingPoint, data);
		}

		void setAlbedoTexture(const std::string& path);
		void setAlbedoTexture(const std::shared_ptr<Texture>& texture);

		void setSpecularTexture(const std::string& path);
		void setSpecularTexture(const std::shared_ptr<Texture>& texture);
	};
}

