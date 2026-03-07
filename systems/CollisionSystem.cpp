#include "CollisionSystem.h"
#include "SpaceComponents.h"

#if OCT_SERVER
// Check collision between two circles
bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)
{
    bool collision = false;

    float dx = center2.x - center1.x; // X distance between centers
    float dy = center2.y - center1.y; // Y distance between centers

    float distanceSquared = dx * dx + dy * dy; // Distance between centers squared
    float radiusSum = radius1 + radius2;

    collision = (distanceSquared <= (radiusSum * radiusSum));

    return collision;
}
#endif

void CollisionSystem::update(entt::registry& registry)
{
    const auto view = registry.view<const SphereCollision, const Position, CollisionResolver>();

    view.each(
            [&registry](entt::entity main_ent,
                        const SphereCollision collision,
                        const Position pos,
                        CollisionResolver& resolver)
            {
                resolver.hit_entities.clear();
                const auto per_view = registry.view<const SphereCollision, const Position>();
                per_view.each(
                        [&collision, &pos, &resolver, &main_ent](
                                const entt::entity entity, const SphereCollision other_coll, const Position other_pos)
                        {
                            if (main_ent == entity)
                            {
                                return;
                            }

                            bool const same_channel = collision.responce_channel == other_coll.channel;
                            bool const result = CheckCollisionCircles(
                                    {pos.x, pos.y}, collision.radius, {other_pos.x, other_pos.y}, other_coll.radius);
                            if (result && same_channel)
                            {
                                resolver.hit_entities.push_back(entity);
                            }
                        });
            });
}
