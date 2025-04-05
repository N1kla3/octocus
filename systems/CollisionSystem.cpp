#include "CollisionSystem.h"
#include "SpaceComponents.h"
#include "raylib.h"

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
