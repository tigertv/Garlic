#include "Tunic/ModelLoader.hpp"

#include "Tunic/Application.hpp"
#include "Tunic/Rendering/Renderables/Mesh.hpp"

#include <Clove/Graphics/Core/Resources/Texture.hpp>
#include <Clove/Graphics/Core/VertexLayout.hpp>
#include <Clove/Platform/Core/Window.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace clv;

namespace tnc::ModelLoader {
	static std::shared_ptr<gfx::Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type) {
		std::shared_ptr<gfx::Texture> texture;

		//TODO: Support multiple textures of the same type
		if(material->GetTextureCount(type) > 0) {
			aiString path;
			material->GetTexture(type, 0, &path);

			gfx::GraphicsFactory& factory = Application::get().getGraphicsFactory();
			gfx::TextureDescriptor descriptor{};

			texture = factory.createTexture(descriptor, path.C_Str());
		}

		return texture;
	}

	static std::shared_ptr<rnd::Mesh> processMesh(aiMesh* mesh, const aiScene* scene) {
		gfx::VertexLayout layout;
		if(mesh->HasPositions()) {
			layout.add(gfx::VertexElementType::position3D);
		}
		if(mesh->HasNormals()) {
			layout.add(gfx::VertexElementType::normal);
		}
		//if(mesh->HasTextureCoords(0)) {
		layout.add(gfx::VertexElementType::texture2D);
		//}
		//Skipping colours for now
		/*if(mesh->HasVertexColors(0)) {
			layout.add(gfx::VertexElementType::colour3D);
		}*/

		gfx::VertexBufferData vertexBufferData{ layout };
		std::vector<uint32_t> indices;
		std::shared_ptr<rnd::Material> meshMaterial = std::make_shared<rnd::Material>();

		const size_t vertexCount = mesh->mNumVertices;
		vertexBufferData.resize(vertexCount);

		for(size_t i = 0; i < vertexCount; ++i) {
			if(mesh->HasPositions()) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::position3D>() = {
					mesh->mVertices[i].x,
					mesh->mVertices[i].y,
					mesh->mVertices[i].z
				};
			}
			if(mesh->HasNormals()) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::normal>() = {
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
				};
			}
			if(mesh->HasTextureCoords(0)) {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::texture2D>() = {
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			} else {
				vertexBufferData[i].getAttribute<gfx::VertexElementType::texture2D>() = {
					0.0f,
					0.0f
				};
			}
		}

		for(size_t i = 0; i < mesh->mNumFaces; ++i) {
			aiFace face = mesh->mFaces[i];
			for(size_t j = 0; j < face.mNumIndices; ++j) {
				indices.emplace_back(face.mIndices[j]);
			}
		}

		if(mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto diffuseTexture = loadMaterialTexture(material, aiTextureType_DIFFUSE);
			auto specularTexture = loadMaterialTexture(material, aiTextureType_SPECULAR);

			if(diffuseTexture) {
				meshMaterial->setAlbedoTexture(std::move(diffuseTexture));
			}
			if(specularTexture) {
				meshMaterial->setSpecularTexture(std::move(specularTexture));
			}
		}

		return std::make_shared<rnd::Mesh>(vertexBufferData, indices, meshMaterial->createInstance());
	}

	static void processNode(aiNode* node, const aiScene* scene, std::vector<std::shared_ptr<rnd::Mesh>>& meshes) {
		for(size_t i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.emplace_back(processMesh(mesh, scene));
		}

		for(size_t i = 0; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene, meshes);
		}
	}

	rnd::Model loadModel(std::string_view filePath) {
		std::vector<std::shared_ptr<rnd::Mesh>> meshes;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filePath.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
		if(scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mRootNode == nullptr) {
			CLV_LOG_ERROR("Assimp Error: {0}", importer.GetErrorString());
			return { meshes };
		}

		processNode(scene->mRootNode, scene, meshes);

		return { meshes };
	}
}