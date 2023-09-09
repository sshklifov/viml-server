#pragma once

#include <rapidjson/document.h>

#include <FStr.hpp>
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

    explicit ValueReader(const rapidjson::Value& val) : val(&val) {}

    const rapidjson::Value&  getValue() const { return *val; }

    template <typename T>
    void readMember(const char* name, T& res) {
        MemberScope scope = beginMember(name);
        read(res);
    }

    template <typename T>
    void readMember(const char* name, std::vector<T>& res) {
        MemberScope scope = beginMember(name);

        const rapidjson::Value* arr = val;
        res.resize(arr->Size());
        for (int i = 0; i < arr->Size(); ++i) {
            val = &arr[i];
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
    template <typename T, typename std::enable_if<std::is_class<T>::value, bool>::type = true>
    void read(T& res) {
        res.read(*this);
    }

    void read(int& res) {
        res = val->GetInt();
    }

    void read(bool& res) {
        res = val->GetBool();
    }

    void read(FStr& res) {
        res = val->GetString();
    }

    MemberScope beginMember(const char* name) {
        return MemberScope(*this, name);
    }

    const rapidjson::Value* val;
};
