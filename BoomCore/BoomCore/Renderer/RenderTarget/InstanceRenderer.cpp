#include "InstanceRenderer.h"

#include "../Manager/MeshMgr.h"
#include "../Manager/ShaderMgr.h"
#include "../Manager/TextureMgr.h"

#include <iostream>

namespace Kawaii
{
	InstanceRenderer::InstanceRenderer(unsigned int shaderIndex)
	{
		m_shaderIndex = shaderIndex;
		glGenBuffers(1, &m_instanceVBO);
	}

	InstanceRenderer::~InstanceRenderer()
	{
		glDeleteBuffers(1, &m_instanceVBO);
	}

	void InstanceRenderer::setInstanceMatrix(const std::vector<glm::mat4>& instanceMatrice, GLenum drawWay)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrice.size() * sizeof(glm::mat4),
			&instanceMatrice[0], drawWay);
		MeshMgr::ptr meshMgr = MeshMgr::getSingleton();
		for (int x = 0; x < m_meshIndex.size(); ++x)
		{
			unsigned int vao = meshMgr->getMesh(m_meshIndex[x])->getVertexArrayObject();
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
			// vertex property.
			GLsizei vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_instance = true;
		m_instanceNum = instanceMatrice.size();
	}

	void InstanceRenderer::render(Camera3D::ptr camera, Light::ptr sunLight,
		Camera3D::ptr lightCamera, Shader::ptr shader)
	{
		if (!m_visiable) return;
		// render the model.
		if (shader == nullptr)
			shader = ShaderMgr::getSingleton()->getShader(m_shaderIndex);
		shader->bind();
		if (sunLight)
			sunLight->setLightUniform(shader, camera);
		shader->setInt("image", 0);

		if (lightCamera != nullptr)
			shader->setMat4("lightSpaceMatrix",
				lightCamera->getProjectMatrix() * lightCamera->getViewMatrix());
		else
			shader->setMat4("lightSpaceMatrix", glm::mat4(1.0f));
		// object matrix.
		shader->setBool("instance", true);
		shader->setBool("receiveShadow", m_receiveShadow);
		shader->setMat4("modelMatrix", m_transformation.getWorldMatrix());
		shader->setMat4("viewMatrix", camera->getViewMatrix());
		shader->setMat4("projectMatrix", camera->getProjectMatrix());
		this->renderImp();
		ShaderMgr::getSingleton()->unBindShader();
	}

}