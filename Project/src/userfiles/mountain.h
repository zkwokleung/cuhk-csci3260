#pragma once
#include "../myapi.h"
#include "../engine/Resources.h"
#include "../engine/Random.h"

class Mountain : public Object {
public:
	Mountain();
	~Mountain();

	virtual void SetActive(bool active);

private:
	ModelObject* m_model;

	static Model* s_mesh;
	static Texture* s_texture;
};