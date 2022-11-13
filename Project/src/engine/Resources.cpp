#include "Resources.h"

std::string Resources::LoadTextFile(std::string path)
{
	std::ifstream myInput(RESOURCES_PATH + path);
	if (!myInput.good())
	{
		Debug::Error("File failed to load " + path);
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>()
	);

}

Mesh* Resources::LoadObject(std::string path)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(RESOURCES_PATH + path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	std::string errorString = importer.GetErrorString();
	if (scene != nullptr && errorString != "") {
		Debug::Error(errorString);
		return nullptr;
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Get vertices
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			Vertex vertex;
			glm::vec3 vector;

			// Get position
			vector.x = scene->mMeshes[i]->mVertices[j].x;
			vector.y = scene->mMeshes[i]->mVertices[j].y;
			vector.z = scene->mMeshes[i]->mVertices[j].z;
			vertex.position = vector;

			// Get normal
			vector.x = scene->mMeshes[i]->mNormals[j].x;
			vector.y = scene->mMeshes[i]->mNormals[j].y;
			vector.z = scene->mMeshes[i]->mNormals[j].z;
			vertex.normal = vector;

			// Get UV
			if (scene->mMeshes[i]->mTextureCoords[0])
			{
				glm::vec2 uv;
				uv.x = scene->mMeshes[i]->mTextureCoords[0][j].x;
				uv.y = scene->mMeshes[i]->mTextureCoords[0][j].y;

				vertex.uv = uv;
			}
			else
			{
				vertex.uv = glm::vec2(0.f);
			}

			vertices.push_back(vertex);
		}

		// Get indices
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (unsigned int k = 0; k < scene->mMeshes[i]->mFaces[j].mNumIndices; k++)
			{
				indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[k]);
			}
		}
	}

	return new Mesh(vertices, indices, nullptr);
}

ImageData* Resources::LoadImageData(std::string path)
{
	ImageData* image = new ImageData();

	// load the texture data into "data"
	std::string fullPath = RESOURCES_PATH + path;
	image->data = SOIL_load_OGL_texture(fullPath.c_str(),
		SOIL_LOAD_AUTO,
		image->data,
		SOIL_FLAG_DDS_LOAD_DIRECT);

	if (!image->data)
	{
		std::stringstream msg;
		msg << "Failed to load texture: " << path;
		Debug::Error(msg.str());
		exit(1);
	}

	std::stringstream msg;
	msg << "Load " << fullPath << " successfully!";
	Debug::Log(msg.str());

	return image;
}

GLuint64 Resources::LoadCubemap(std::vector<std::string> paths)
{
	// Load image with SOIL
	GLuint tex_cube = SOIL_load_OGL_cubemap
	(
		(RESOURCES_PATH + paths[0]).c_str(),
		(RESOURCES_PATH + paths[1]).c_str(),
		(RESOURCES_PATH + paths[2]).c_str(),
		(RESOURCES_PATH + paths[3]).c_str(),
		(RESOURCES_PATH + paths[4]).c_str(),
		(RESOURCES_PATH + paths[5]).c_str(),
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS
	);

	if (tex_cube == 0)
	{
		std::stringstream msg;
		msg << "Cubemap tex failed to load at paths: ";
		for (unsigned int i = 0; i < 6; i++)
		{
			msg << RESOURCES_PATH + paths[i] << ", ";
		}
		Debug::Error(msg.str());
	}

	return tex_cube;
}

void Resources::FreeImage(ImageData* image)
{
	if (image->data)
	{

	}
}

