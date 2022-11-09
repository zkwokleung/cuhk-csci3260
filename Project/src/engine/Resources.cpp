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

Model Resources::LoadObject(std::string path)
{
	// function to load the obj file
	// Note: this simple function cannot load all obj files.

	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;

	std::string fullpath = RESOURCES_PATH + path;
	std::cout << "\nLoading OBJ file " << fullpath << "..." << std::endl;

	std::ifstream file;
	file.open(fullpath);

	// Check for Error
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path?" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();

	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
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

