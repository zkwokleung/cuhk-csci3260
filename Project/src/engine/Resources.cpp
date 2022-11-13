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
	// tell stb_image.h to flip loaded texture's on the y-axis.
	stbi_set_flip_vertically_on_load(true);
	// load the texture data into "data"
	std::string fullPath = RESOURCES_PATH + path;
	image->data = stbi_load(fullPath.c_str(), &image->Width, &image->Height, &image->BPP, 0);

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

std::vector<ImageData*> Resources::LoadCubemap(std::vector<std::string> paths)
{
	std::vector<ImageData*> results;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		ImageData* image = new ImageData();

		std::string fullPath = RESOURCES_PATH + paths[i];
		image->data = stbi_load(fullPath.c_str(), &image->Width, &image->Height, &image->BPP, 0);
		if (!image->data)
		{
			std::stringstream msg;
			msg << "Cubemap tex failed to load at path: " << paths[i];
			Debug::Error(msg.str());
			FreeImage(image);
		}

		std::stringstream msg;
		msg << "Load " << fullPath << " successfully!";
		Debug::Log(msg.str());
		results.push_back(image);
	}

	return results;
}

void Resources::FreeImage(ImageData* image)
{
	if (image->data)
	{
		stbi_image_free(image->data);
	}
}

