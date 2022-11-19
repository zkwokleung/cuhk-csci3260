#pragma once
#include "../renders/shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <list>

class Transform
{
  public:
    Transform();
    ~Transform();

    // World Space. Parent's transform will be taken into account
    glm::vec3 GetPosition() const;
    void SetPosition(glm::vec3 value);

    glm::vec3 GetRotation() const;
    void SetRotation(glm::vec3 value);

    glm::vec3 GetScale() const;
    void SetScale(glm::vec3 value);

    // Local space
    glm::vec3 GetLocalPosition() const;
    void SetLocalPosition(glm::vec3 value);

    glm::vec3 GetLocalRotation() const;
    void SetLocalRotation(glm::vec3 value);

    glm::vec3 GetLocalScale() const;
    void SetLocalScale(glm::vec3 value);

    glm::mat4 GetTransformMat4() const;
    glm::mat4 GetRotationMat4() const;
    glm::mat4 GetScaleMat4() const;
    glm::mat4 GetPositionMat4() const;

    glm::vec3 GetForward() const;
    glm::vec3 GetBackward() const;
    glm::vec3 GetLeft() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;
    glm::vec3 GetDown() const;

    Transform *GetParent();
    void SetParent(Transform *transform);
    std::list<Transform *> GetChilds() const;

    virtual void OnPaint(Shader *shader);

  private:
    glm::vec3 m_localPosition;
    glm::vec3 m_localRotation;
    glm::vec3 m_localScale;

    Transform *m_parent;
    std::list<Transform *> m_childs;
};