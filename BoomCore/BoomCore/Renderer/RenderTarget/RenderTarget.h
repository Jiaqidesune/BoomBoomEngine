#pragma once

#include <vector>
#include <memory>

#include "../Manager/Geometry.h"
#include "../Light.h"
#include "../Camera/Camera3D.h"
#include "../Camera/Transform3D.h"

namespace Kawaii
{

	class PBRMaterial
	{
	public:
		unsigned int m_albedoTexIndex = -1;
		unsigned int m_normalTexIndex = -1;
		unsigned int m_roughTexIndex = -1;
		unsigned int m_metallicIndex = -1;
	};

	class RenderTarget
	{
	protected:
		bool m_instance = false;
		bool m_receiveShadow = true;
		bool m_visiable = true;
		int m_instanceNum = 0;
		Transform3D m_transformation;

		unsigned int m_shaderIndex;
		std::vector<unsigned int> m_texIndex;
		std::vector<unsigned int> m_meshIndex;
		std::vector<PBRMaterial> m_PBRtexIndex;

	public:
		typedef std::shared_ptr<RenderTarget> ptr;

		RenderTarget() = default;
		virtual ~RenderTarget() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr) = 0;

		virtual void getAABB(glm::vec3& min, glm::vec3& max) {}

		void setVisiable(bool target) { m_visiable = target; }
		void setReceiveShadow(bool target) { m_receiveShadow = target; }

		void addPbrTexture(PBRMaterial matIndex) { m_PBRtexIndex.push_back(matIndex); }

		void addTexture(unsigned int texIndex)
		{
			m_texIndex.push_back(texIndex);
		}

		void addMesh(unsigned int meshIndex)
		{
			m_meshIndex.push_back(meshIndex);
		}

		Transform3D* getTransformation()
		{
			return &m_transformation;
		}

	protected:
		void renderImp();
	};

	class RenderTargetList : public RenderTarget
	{
	private:
		std::vector<RenderTarget::ptr> m_list;

	public:
		typedef std::shared_ptr<RenderTargetList> ptr;

		RenderTargetList() = default;
		virtual ~RenderTargetList() = default;

		unsigned int addRenderer(RenderTarget* object)
		{
			RenderTarget::ptr RenderTarget(object);
			m_list.push_back(RenderTarget);
			return m_list.size() - 1;
		}

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr)
		{
			for (auto& it : m_list)
				it->render(camera, sunLight, lightCamera, shader);
		}

	};

	class SkyDome : public RenderTarget
	{
	public:
		typedef std::shared_ptr<SkyDome> ptr;

		SkyDome(unsigned int shaderIndex)
		{
			m_shaderIndex = shaderIndex;
		}

		SkyDome() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	
	};

	class SimpleObject : public RenderTarget
	{
	public:

		SimpleObject(unsigned int shaderIndex)
		{
			m_shaderIndex = shaderIndex;
		}

		~SimpleObject() = default;

		virtual void render(Camera3D::ptr camera, Light::ptr sunLight, Camera3D::ptr lightCamera, Shader::ptr shader = nullptr);
	
	};
}

//