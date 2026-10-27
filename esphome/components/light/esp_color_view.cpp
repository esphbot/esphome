#include "esp_color_view.h"

namespace esphome::light {

NullColorAccessor NullColorAccessor::instance{};
#ifdef USE_LIGHT_LEGACY_VIEW
PointerColorAccessor PointerColorAccessor::scratch{};
#endif

}  // namespace esphome::light
