#include "UI.h"

#include <imgui.h>

namespace GLReady {

    namespace ui {

        void SetFont(const std::string &fontPath, int sizeInPixels)
        {
            auto &io = ImGui::GetIO();

            ImFontConfig fontConfig;
            fontConfig.MergeMode = true;
            fontConfig.PixelSnapH = true;
            fontConfig.GlyphOffset = ImVec2(0.0f, 6.5f);
            io.Fonts->AddFontFromFileTTF(fontPath.c_str(), sizeInPixels, NULL,
                                         io.Fonts->GetGlyphRangesDefault());
        }

    } // namespace ui

} // namespace GLReady
