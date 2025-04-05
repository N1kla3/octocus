#include "CollisionSystem.h"
#include "SpaceComponents.h"
#include "raylib.h"

void CollisionSystem::update(entt::registry& registry)
{
    auto view = registry.view<const sphere_collision, const position, collision_resolver>();

    view.each(
            [&registry](entt::entity main_ent,
                        const sphere_collision collision,
                        const position pos,
                        collision_resolver& resolver)
            {
                resolver.hit_entities.clear();
                auto per_view = registry.view<const sphere_collision, const position>();
                per_view.each(
                        [&collision, &pos, &resolver, &main_ent](
                                entt::entity entity, const sphere_collision other_coll, const position other_pos)
                        {
                            if (main_ent == entity)
                                return;

                            bool same_channel = collision.responce_channel == other_coll.channel;
                            bool result = CheckCollisionCircles(
                                    {pos.x, pos.y}, collision.radius, {other_pos.x, other_pos.y}, other_coll.radius);
                            if (result && same_channel)
                            {
                                resolver.hit_entities.push_back(entity);
                            }
                        });
            });
}
