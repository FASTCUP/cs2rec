#include <ISmmPlugin.h>

namespace Interfaces {
    void Create();
};
void WrpRegisterCommands();

class CS2Rec final : public ISmmPlugin, public IMetamodListener
{
public:
    bool Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late) {
        Interfaces::Create();
        WrpRegisterCommands();
        return true;
    }
private:
    const char* GetAuthor() { return ""; }
    const char* GetName() { return ""; }
    const char* GetDescription() { return ""; }
    const char* GetURL() { return ""; }
    const char* GetLicense() { return ""; }
    const char* GetVersion() { return ""; }
    const char* GetDate() { return ""; }
    const char* GetLogTag() { return ""; }
} s_cs2Rec;

PLUGIN_EXPOSE(CS2Rec, s_cs2Rec);
