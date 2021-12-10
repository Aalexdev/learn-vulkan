#pragma once

#include "Model.hpp"

// std
#include <memory>

namespace ECS{
    namespace Components{

        struct Model{
            std::shared_ptr<VKE::Model> model{nullptr};
        };

    } // namespace Components
} // namespace ECS