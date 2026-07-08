#include "BindableCodex.h"

std::shared_ptr<Bindable> Codex::Resolve(
    const std::string& key
) conexcept
{
    return Get().Resolve_(key);
}

void Codex::Store(
    std::shared_ptr<Bindable> bind
)
{
    Get().Store_(std::move(bind));
}

std::shared_ptr<Bindable> Codex::Resolve_(
    const std::string& key
) const conexcept
{
    auto i = binds.find(key);

    if (i == binds.end())
    {
        return {};
    }

    return i->second;
}

void Codex::Store_(
    std::shared_ptr<Bindable> bind
)
{
    binds[bind->GetUID()] = std::move(bind);
}

Codex& Codex::Get()
{
    static Codex codex;
    return codex;
}