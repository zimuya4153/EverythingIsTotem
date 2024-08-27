#pragma once

#include "Entry.h"

#include <ll/api/memory/Hook.h>
#include <ll/api/mod/RegisterHelper.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/item/VanillaItemNames.h>
#include <mc/deps/core/string/HashedString.h>
#include <mc/world/item/ItemStackBase.h>
#include <memory>

namespace EverythingIsTotem {

static std::unique_ptr<Entry> instance;

Entry& Entry::getInstance() { return *instance; }

bool Entry::load() { return true; }

bool Entry::enable() { return true; }

bool Entry::disable() { return true; }

bool Entry::unload() { return true; }

} // namespace EverythingIsTotem

LL_REGISTER_MOD(EverythingIsTotem::Entry, EverythingIsTotem::instance);

LL_AUTO_TYPE_INSTANCE_HOOK(
    ItemIsInstanceHook,
    HookPriority::Normal,
    ItemStackBase,
    &ItemStackBase::isInstance,
    bool,
    const HashedString& itemName,
    bool                useItemLookup
) {
    if (itemName == VanillaItemNames::TotemOfUndying) return true;
    return origin(itemName, useItemLookup);
}

LL_AUTO_TYPE_INSTANCE_HOOK(ConsumeTotemHook, HookPriority::Normal, Player, "?consumeTotem@Player@@UEAA_NXZ", bool) {
    if (auto item = getSelectedItem(); !item.isNull()) {
        item.mCount--;
        setSelectedItem(item);
        refreshInventory();
        return true;
    }
    if (auto item = getOffhandSlot(); !item.isNull()) {
        item.mCount--;
        setOffhandSlot(item);
        refreshInventory();
        return true;
    }
    return false;
}