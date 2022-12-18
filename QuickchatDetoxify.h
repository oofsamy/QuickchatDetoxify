#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "GuiBase.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

const std::vector<std::string> QuickChats = { "$#@%!", "All yours.", "Bumping!", "Calculated.", "Centering!", "Close one!", "Defending...", "Everybody dance!", "Faking.", "Get ready.", "gg", "Go for it!", "Good luck!", "Great clear!", "Great pass!", "Have fun!", "Here. We. Go.", "Holy cow!", "I got it!", "I'll do my best.", "In position.", "Incoming!", "Let's do this!", "My bad...", "My fault.", "Need boost!", "Nice block!", "Nice block!",  "Nice cars!", "Nice demo!", "Nice moves.", "Nice one!", "Nice shot!", "No problem.", "No way!", "Noooo!", "OMG!", "Okay.", "On your left.", "On your right.", "One. More. Game.", "Oops!", "Passing!", "Rematch!", "Savage!", "Siiiick!", "Sorry!", "Take the shot!", "Thanks!", "That was fun!", "This is Rocket League!", "What a play!", "What a save!", "Whew.", "Whoops...", "Wow!", "Well Played.", "What a game!"};

class QuickchatDetoxify: public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase {
	void onLoad() override;

	bool BlockMessages = false;

	std::vector<std::string> MessagesBlacklist;
	std::vector<std::string> PeopleBlacklist;
public:
	void RenderSettings() override;
};
