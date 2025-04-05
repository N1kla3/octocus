#include "AnimationSystem.h"
#include "Animation.h"
#include "raylib.h"

void AnimationSystem::updateAnimation(entt::registry& registry, float deltaTime)
{
    auto text_anim = registry.view<TextAnimation>();

    text_anim.each(
            [&registry, deltaTime](entt::entity ent, TextAnimation& anim)
            {
                if (anim.current_time > anim.max_time)
                {
                    registry.destroy(ent);
                    return;
                }

                anim.current_time += deltaTime;

                float font_size = anim.font_size;
                float spacing = 1.f;
                Vector2 pos = anim.pos;
                DrawTextEx(GetFontDefault(), anim.text.c_str(), pos, font_size, spacing, anim.color);
            });
}
