#pragma once

#include "Bindable.h"
#include <memory>
#include <unordered_map>

class Codex
{
public:
    static std::shared_ptr<Bindable> Resolve(const std::string& key) conexcept;

    static void Store(std::shared_ptr<Bindable> bind);

private:
    std::shared_ptr<Bindable> Resolve_(const std::string& key) const conexcept;

    void Store_(std::shared_ptr<Bindable> bind);

    static Codex& Get();

private:
    std::unordered_map<std::string, std::shared_ptr<Bindable>> binds;
};
