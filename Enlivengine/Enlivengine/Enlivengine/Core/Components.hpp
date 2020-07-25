#pragma once

#include <entt/entt.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Core/ComponentDependencyTrait.hpp>
#include <Enlivengine/Core/Entity.hpp>

#include <string>
#include <Enlivengine/Core/Transform.hpp>
#include <Enlivengine/Graphics/Sprite.hpp>
#include <Enlivengine/Graphics/Text.hpp>

namespace en
{

struct NameComponent
{
	NameComponent() : name("") {}
	NameComponent(const char* pName) : name(pName) {}
	NameComponent(const std::string& pName) : name(pName) {}

	std::string name;
};

struct UIDComponent
{
	UIDComponent() {}
	UIDComponent(U32 pId) : id(pId) {}

	U32 id;
};

struct RenderableComponent
{
	RenderableComponent() : id(0) {}

	U32 id; // Unused, just that entt needs component to not be empty 
};

struct TransformComponent
{
	TransformComponent() : transform() {}
	TransformComponent(const Transform& pTransform) : transform(pTransform) {}

	Transform transform;
};

struct SpriteComponent
{
	SpriteComponent() : sprite() {}
	SpriteComponent(const Sprite& pSprite) : sprite(pSprite) {}

	Sprite sprite;
};

struct TextComponent
{
	TextComponent() : text() {}
	TextComponent(const Text& pText) : text(pText) {}

	Text text;
};

struct ChildComponent
{
	ChildComponent() : parentEntity(entt::null) {}
	ChildComponent(const entt::entity& pParentEntity) : parentEntity(pParentEntity) {}

	entt::entity parentEntity;
};

struct ParentComponent
{
	ParentComponent() : childEntities() {}

	std::vector<entt::entity> childEntities;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::NameComponent)
	ENLIVE_META_CLASS_MEMBER("name", &en::NameComponent::name)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::UIDComponent)
	ENLIVE_META_CLASS_MEMBER("id", &en::UIDComponent::id)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::RenderableComponent)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::TransformComponent)
	ENLIVE_META_CLASS_MEMBER("transform", &en::TransformComponent::transform)
	ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::SpriteComponent)
	ENLIVE_META_CLASS_MEMBER("sprite", &en::SpriteComponent::sprite)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::TextComponent)
	ENLIVE_META_CLASS_MEMBER("text", &en::TextComponent::text)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::ChildComponent)
ENLIVE_META_CLASS_END()

ENLIVE_META_CLASS_BEGIN(en::ParentComponent)
ENLIVE_META_CLASS_END()

// SpriteComponent needs RenderableComponent
template <>
struct ComponentDependencyTrait<en::SpriteComponent>
{
	static constexpr bool hasDependency = true;
	static bool CanEntityHaveThisComponent(const en::Entity& entity)
	{
		return entity.Has<en::RenderableComponent>();
	}
}; 

// TextComponent needs RenderableComponent
template <>
struct ComponentDependencyTrait<en::TextComponent>
{
	static constexpr bool hasDependency = true;
	static bool CanEntityHaveThisComponent(const en::Entity& entity)
	{
		return entity.Has<en::RenderableComponent>();
	}
};

// ChildComponent needs UIDComponent
template <>
struct ComponentDependencyTrait<en::ChildComponent>
{
	static constexpr bool hasDependency = true;
	static bool CanEntityHaveThisComponent(const en::Entity& entity)
	{
		return entity.Has<en::UIDComponent>();
	}
};

// ParentComponent needs UIDComponent
template <>
struct ComponentDependencyTrait<en::ParentComponent>
{
	static constexpr bool hasDependency = true;
	static bool CanEntityHaveThisComponent(const en::Entity& entity)
	{
		return entity.Has<en::UIDComponent>();
	}
};
