#include "WorldMap.h"
#include "Misc/AsmPatch.h"
#include "LuaMain/LunaLuaMain.h"
#include "Globals.h"
#include "Misc/LoadScreen.h"

#include "SMBXInternal/Functions.h"
#include "SMBXInternal/Variables.h"
#include "SMBXInternal/Types.h"

bool mapOverrideEanbled = false;

static void __stdcall worldLoopHook() {
    // allow lua to handle the world map loop
    if (gLunaLua.isValid()) {
        std::shared_ptr<Event> worldDrawEvent = std::make_shared<Event>("onOverworldSystemTick", false);
        worldDrawEvent->setDirectEventName("onOverworldSystemTick");
        worldDrawEvent->setLoopable(false);
        gLunaLua.callEvent(worldDrawEvent);
    }
    // if the world map was exited, check to load level
    if (GM_EPISODE_MODE == 0) {
        // if a level to load was set
        if (GM_NEXT_LEVEL_FILENAME.length() > 0) {
            LunaLoadScreenStart(); // show load screen
            native_cleanupLevel(); // clear level
            
            // get full filename for level to load
            GM_NEXT_LEVEL_FILENAME = std::string(GM_FULLDIR) + std::string(GM_NEXT_LEVEL_FILENAME);
            // load the level
            native_loadLevel(&GM_NEXT_LEVEL_FILENAME);

            // reset 
            GM_NEXT_LEVEL_FILENAME = "";
        }
    }
}


// Patch to override SMBX 1.3 'WorldLoop',
// this is right after tha call to UpdateGraphics2/UpdateControls/UpdateSound
//static auto overrideWorldLoopPatch = PATCH(0x8E088A).CALL(worldLoopHook).JMP(0x8E1DB7).NOP_PAD_TO_SIZE<20>();

// set world map override to active or inactive
// this will switch the entire world map rendering and logic to be handled by Lua
void WorldMap::SetWorldMapOverrideEnabled(bool enabled)
{
    if (mapOverrideEanbled != enabled)
    {
        mapOverrideEanbled = enabled;
        if (enabled)
        {
            // enable world map override
            //overrideWorldLoopPatch.Apply();
        }
        else
        {
            // disable world map override
            //overrideWorldLoopPatch.Unapply();
        }
    }
}

bool WorldMap::GetWorldMapOverrideEnabled()
{
    return mapOverrideEanbled;
}

void WorldMap::openPath(int winState, int levelDirection, bool useAnimation)
{
    using namespace SMBX13::Functions;
    using namespace SMBX13::Vars;
    using namespace SMBX13::Types;

    if(winState <= -2)
    {
        winState = -1;
    }

    auto& level = WorldLevel[curWorldLevel];
    if(levelDirection <= 0 || levelDirection >= 5)
    {
        for(int i = 1; i <= 4; i++)
        {
            if(level.LevelExit[i] == (int16_t)winState || winState == -1)
            {
                LevelPath((int16_t)curWorldLevel, (int16_t)i, useAnimation);
            }
        }
    }
    else
    {
        if(level.LevelExit[levelDirection] == (int16_t)winState || winState == -1)
        {
            LevelPath((int16_t)curWorldLevel, (int16_t)levelDirection, useAnimation);
        }
    }
}