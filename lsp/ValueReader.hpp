#pragma once

#include <rapidjson/document.h>

#include <vector>
#include <optional>

struct ValueReader {
    struct MemberScope {
        MemberScope(ValueReader& r, const char* name) : reader(r), origVal(r.val) {
            rapidjson::Value::ConstMemberIterator it = r.val->FindMember(name);
            if (it != r.val->MemberEnd()) {
                r.val = &it->value;
            } else {
                assert(false);
            }
        }

        ~MemberScope() {
            reader.val = origVal;
        }

        ValueReader& reader;
        const rapidjson::Value* origVal;
    };

    ValueReader(const rapidjson::Value& val) : val(&val) {}

    MemberScope beginMember(const char* name) {
        return MemberScope(*this, name);
    }

    template <typename T>
    void read(T& res) {
        if (val->Is<T>()) {
            res = val->Get<T>();
        }
    }

    template <typename T>
    void readMember(const char* name, T& res) {
        MemberScope scope = beginMember(name);
        read(res);
    }

    template <typename T>
    void readMember(const char* name, std::vector<T>& res) {
        MemberScope scope = beginMember(name);

        res.resize(val->Size());
        const rapidjson::Value* oldVal = val;
        for (int i = 0; i < val->Size(); ++i) {
            val = &oldVal[i];
            read(res[i]);
        }
    }

    template <typename T>
    void readMember(const char* name, std::optional<T>& res) {
        MemberScope scope = beginMember(name);
        if (!val->IsNull()) {
            read(res.value());
        }
    }

private:
    const rapidjson::Value* val;
};
