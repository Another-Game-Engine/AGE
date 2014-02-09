#pragma once

#define TAG(x) (std::static_assert(x < 32 && x >= 0 && "Only 32 tags are alowed");char 1 << x)