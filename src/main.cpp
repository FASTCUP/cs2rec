#include <ISmmPlugin.h>

namespace Interfaces {
    bool Create();
};

class CS2Rec final : public ISmmPlugin, public IMetamodListener
{
public:
    bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) override;

private:
    const char* GetAuthor() override    { return "cademtz"; }
    const char* GetName() override      { return "cs2rec"; }
    const char* GetDescription() override { return "FFmpeg recording commands"; }
    const char* GetURL() override       { return "https://github.com/cademtz/cs2rec"; }
    const char* GetLicense() override   { return ""; }
    const char* GetVersion() override   { return ""; }
    const char* GetDate() override      { return __DATE__; }
    const char* GetLogTag() override    { return ""; }
} s_cs2Rec;

PLUGIN_EXPOSE(CS2Rec, s_cs2Rec);

bool CS2Rec::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) {
    PLUGIN_SAVEVARS();
    if (!Interfaces::Create())
        return false;
    ConVar_Register(FCVAR_RELEASE);
    return true;
}