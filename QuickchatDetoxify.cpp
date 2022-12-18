#include "pch.h"
#include "QuickchatDetoxify.h"


BAKKESMOD_PLUGIN(QuickchatDetoxify, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

struct FString
{
public:
    using ElementType = const wchar_t;
    using ElementPointer = ElementType*;

private:
    ElementPointer ArrayData;
    int32_t ArrayCount;
    int32_t ArrayMax;

public:
    FString()
    {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;
    }

    FString(ElementPointer other)
    {
        ArrayData = nullptr;
        ArrayCount = 0;
        ArrayMax = 0;

        ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

        if (ArrayCount > 0)
        {
            ArrayData = other;
        }
    }

    ~FString() {}

public:
    std::string ToString() const
    {
        if (!IsValid())
        {
            std::wstring wideStr(ArrayData);
            std::string str(wideStr.begin(), wideStr.end());
            return str;
        }

        return std::string("null");
    }

    bool IsValid() const
    {
        return !ArrayData;
    }

    FString operator=(ElementPointer other)
    {
        if (ArrayData != other)
        {
            ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

            if (ArrayCount > 0)
            {
                ArrayData = other;
            }
        }

        return *this;
    }

    bool operator==(const FString& other)
    {
        return (!wcscmp(ArrayData, other.ArrayData));
    }
};

FString FS(const std::string& s) {
    wchar_t* p = new wchar_t[s.size() + 1];
    for (std::string::size_type i = 0; i < s.size(); ++i)
        p[i] = s[i];

    p[s.size()] = '\0';
    return FString(p);
}

struct ChatMessage {
    int32_t Team; 
    class FString PlayerName;
    class FString Message;
    uint8_t ChatChannel;
    bool bLocalPlayer : 1;
    unsigned char IDPadding[0x48];
    uint8_t MessageType;
};

std::string Lower(std::string S) {
    for (auto& c : S) {
        c = tolower(c);
    }

    return S;
}

void QuickchatDetoxify::onLoad()
{
	_globalCvarManager = cvarManager;
	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GFxData_Chat_TA.OnChatMessage", [this](ActorWrapper Caller, void* params, ...) {
		ChatMessage* Params = (ChatMessage*)params;
        std::string PlayerName = Params->PlayerName.ToString();
        std::string Content = Params->Message.ToString();

        if (BlockMessages) {
            if (PlayerName != gameWrapper->GetPlayerName().ToString()) {
                for (std::string MessageBlacklist : MessagesBlacklist) {
                    if (Lower(Content).find(Lower(MessageBlacklist)) != std::string::npos) {
                        Params->Message = FS("");
                        Params->PlayerName = FS("");
                    }
                }
            }
        }

	});
}

void QuickchatDetoxify::RenderSettings() {
    static int CurrentItem = 0;

    ImGui::Text("Blocked Messages (not case sensitive)");

    static std::string Con;

    if (ImGui::ListBoxHeader("##h", {240, 280})) {
        for (int n = 0; n < MessagesBlacklist.size(); n++) {
            const bool IsSelected = (CurrentItem == n);

            if (ImGui::Selectable(MessagesBlacklist[n].c_str(), IsSelected)) {
                CurrentItem = n;

                Con = MessagesBlacklist[n];
            }

            if (IsSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::ListBoxFooter();
    }

    ImGui::SetNextItemWidth(240);

    if (ImGui::InputText("", &Con)) { }

    if (ImGui::Button("Add", { 115.25, 20 })) { if (Con != "") { MessagesBlacklist.push_back(Con); LOG("Con: {}", Con); } }

    ImGui::SameLine();

    if (ImGui::Button("Remove", {115.25, 20})) {
        for (int i = 0; i < MessagesBlacklist.size(); i++) {
            if (MessagesBlacklist[i] == Con) {
                MessagesBlacklist.erase(MessagesBlacklist.begin() + i);

                Con = "";
            }
        }
    }

    if (ImGui::Checkbox("Block messages", &BlockMessages)) { }



    static int CurrentQuickChatItem = 0;

    ImGui::SetCursorPos({ 250, 21.25 });

    ImGui::Text("Quick Chats");

    static std::string QuickCon;

    ImGui::SetCursorPos({250, 37.375});

    if (ImGui::ListBoxHeader("##hg", { 240, 280 })) {
        for (int n = 0; n < QuickChats.size(); n++) {
            const bool IsSelected = (CurrentQuickChatItem == n);

            if (ImGui::Selectable(QuickChats[n].c_str(), IsSelected)) {
                CurrentQuickChatItem = n;

                QuickCon = QuickChats[n];
            }

            if (IsSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::ListBoxFooter();
    }

    ImGui::SetNextItemWidth(240);

    ImGui::SetCursorPos({ 250, 321.5 });

    if (ImGui::InputText("##g", &QuickCon)) {}

    ImGui::SetCursorPos({ 250, 345 });

    if (ImGui::Button("Add Quick Chat", { 115.25, 20 })) { if (QuickCon != "") { MessagesBlacklist.push_back(QuickCon); LOG("Con: {}", QuickCon); } }

    ImGui::SameLine();

    if (ImGui::Button("Remove Quick Chat", { 115.25, 20 })) {
        for (int i = 0; i < MessagesBlacklist.size(); i++) {
            if (MessagesBlacklist[i] == QuickCon) {
                MessagesBlacklist.erase(MessagesBlacklist.begin() + i);

                QuickCon = "";
            }
        }
    }
}