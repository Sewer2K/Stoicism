#include "../../includes.hpp"
#include <cstring>
#include <unistd.h>
#include <pwd.h>
#include <vector>

// p100 flex ur distro
char distro[32];
void getDistro() {
    static bool gotDistro = false;
    if (!gotDistro) {
        std::ifstream osRelease("/etc/os-release");
        if (osRelease.is_open()) {
            std::string line;
            while (getline(osRelease, line)) {
                if (strstr(line.c_str(), "ID=") == line.c_str()) {
                    memcpy(distro, line.substr(3).c_str(), 32);
                }
            }
            osRelease.close();
        }
        gotDistro = true;
    }
}

void Menu::drawOverlay(ImDrawList* drawList) {
    getDistro();
    char hostname[64];
    gethostname(hostname, 64);
    Globals::drawList = drawList;
    int w, h;
    Interfaces::engine->GetScreenSize(w, h);

    if(!CONFIGBOOL("Misc>Misc>Misc>Disable Watermark")) {
        int ping = (Interfaces::engine->IsInGame() && playerResource) ? playerResource->GetPing(Interfaces::engine->GetLocalPlayer()) : 0;
        float framePadding = 20.f;
        float textPadding = 5.f;

        char watermarkText[64];
        sprintf(watermarkText, "x352cheats (%s - %s@%s) | %.0f fps | %i ms", distro, getpwuid(getuid())->pw_name, hostname, ImGui::GetIO().Framerate, ping);

        float width = ImGui::CalcTextSize(watermarkText).x + (textPadding * 2);
        float height = 22.f;

        Globals::drawList->AddRectFilled(ImVec2(w - width - framePadding, framePadding), ImVec2(w - framePadding, framePadding + height), ImColor(25, 25, 25));
        Menu::drawBorder(ImVec2(w - width - framePadding, framePadding), ImVec2(width, height), Globals::drawList);

        // Color bar
        ImColor l = ImColor(50, 160, 200),
                m = ImColor(180, 80, 180),
                r = ImColor(190, 210, 50),
                l2 = ImColor(30, 90, 110),
                m2 = ImColor(100, 40, 100),
                r2 = ImColor(70, 80, 20);

        Globals::drawList->AddRectFilledMultiColor(ImVec2(w - width - framePadding + 1, framePadding + 1), ImVec2(w - framePadding - (width / 2) , framePadding + 2), l, m, m, l);
        Globals::drawList->AddRectFilledMultiColor(ImVec2(w - framePadding - (width / 2), framePadding + 1), ImVec2(w - framePadding - 1, framePadding + 2), m, r, r, m);

        Globals::drawList->AddRectFilledMultiColor(ImVec2(w - width - framePadding + 1, framePadding + 2), ImVec2(w - framePadding - (width / 2) , framePadding + 3), l2, m2, m2, l2);
        Globals::drawList->AddRectFilledMultiColor(ImVec2(w - framePadding - (width / 2), framePadding + 2), ImVec2(w - framePadding - 1, framePadding + 3), m2, r2, r2, m2);

        Globals::drawList->AddText(ImVec2(w - width - framePadding + textPadding + 1, height + 1 + (textPadding / 2) + 1), ImColor(0, 0, 0, 255), watermarkText);
        Globals::drawList->AddText(ImVec2(w - width - framePadding + textPadding, height + 1 + (textPadding / 2)), ImColor(255, 255, 255, 255), watermarkText);
    }
        Globals::drawList->AddText(ImVec2((w / 2) + 1, (h / 2) + 1), ImColor(0, 0, 0), "EB");

    Features::ESP::draw();
    Features::RecoilCrosshair::draw();
    Features::Spectators::draw();
    Features::PlayerList::draw();
    Features::FlappyBird::draw();
    Features::Notifications::draw();
    Features::Hitmarkers::draw();
}
