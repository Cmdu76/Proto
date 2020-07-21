#pragma once

#include <entt/entt.hpp>

#include "Entity.hpp"

namespace en
{

class EntityManager
{
public:
	EntityManager();

	Entity CreateEntity();
	void DestroyEntity(Entity& entity);
	void ClearEntities();

	template <typename Func>
	void Each(Func func) const
	{
		mRegistry.each(func);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {}) const
	{
		return mRegistry.view<Component...>(exclusion);
	}

	template <typename... Component, typename... Exclude>
	auto View(entt::exclude_t<Exclude...> exclusion = {})
	{
		return mRegistry.view<Component...>(exclusion);
	}

private:
	friend class Entity;

	entt::registry& GetRegistry() { return mRegistry; }
	const entt::registry& GetRegistry() const { return mRegistry; }

private:
	entt::registry mRegistry;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::EntityManager)
ENLIVE_META_CLASS_END()

// TODO : Figure out how to move it elsewhere
template <>
struct CustomObjectEditor<en::EntityManager>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(en::EntityManager& object, const char* name)
	{
		assert(name != nullptr);
		bool modified = false;
		if (ImGui::CollapsingHeader(name))
		{
			if (ImGui::Button("New Entity"))
			{
				object.CreateEntity();
				modified = true;
			}
			object.Each([&object, &modified](auto entityEntt)
			{
				en::Entity entity(object, entityEntt);
				if (entity.IsValid())
				{
					ImGui::PushID(entity.GetID());
					if (ImGui::Button("x"))
					{
						object.DestroyEntity(entity);
						modified = true;
					}
					else
					{
						ImGui::SameLine();
						if (CustomObjectEditor<en::Entity>::ImGuiEditor(entity, entity.GetName()))
						{
							modified = true;
						}
					}
					ImGui::PopID();
				}
			});
		}
		return modified;
	}
};

// TODO : Figure out how to move it elsewhere
template <>
struct CustomXmlSerialization<en::EntityManager>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const en::EntityManager& object, const char* name)
	{
		auto& parser = dataFile.GetParser();
		if (parser.CreateNode(name))
		{
			bool anyError = false;
			dataFile.WriteCurrentType<en::EntityManager>();
			object.Each([&dataFile, &object, &anyError](auto entityEntt)
			{
				en::Entity entity(const_cast<en::EntityManager&>(object), entityEntt);
				if (entity.IsValid())
				{
					if (!CustomXmlSerialization<en::Entity>::Serialize(dataFile, entity, entity.GetName()))
					{
						anyError = true;
					}
				}
			});
			parser.CloseNode();
			return !anyError;
		}
		else
		{
			return false;
		}
	}
	static bool Deserialize(DataFile& dataFile, en::EntityManager& object, const char* name)
	{
		return true;
	}
};