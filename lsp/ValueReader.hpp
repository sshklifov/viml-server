#pragma once

#include <rapidjson/document.h>

#include <vector>
#include <optional>

struct ValueReader {
    struct MemberScope {
        MemberScope(ValueReader& r, const char* name) : storedReader(r) {
            rapidjson::Value::MemberIterator it = r.val.FindMember(name);
            if (it != r.val.MemberEnd()) {
                storedVal = std::move(r.val);
                r.val = std::move(it->value);
            } else {
                storedVal = std::move(r.val);
                r.val.SetNull();
            }
        }

        ~MemberScope() {
            storedReader.val = std::move(storedVal);
        }

        ValueReader& storedReader;
        rapidjson::Value storedVal;
    };

    ValueReader(rapidjson::Value val) : val(std::move(val)) {}

    MemberScope beginMember(const char* name) {
        return MemberScope(*this, name);
    }

    template <typename T>
    void read(T& res) {
        if (val.Is<T>()) {
            res = val.Get<T>();
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

        res.resize(val.Size());
        rapidjson::Value arr = std::move(val);
        for (int i = 0; i < val.Size(); ++i) {
            val = arr[i];
            read(res[i]);
        }
    }

    template <typename T>
    void readMember(const char* name, std::optional<T>& res) {
        MemberScope scope = beginMember(name);
        if (!val.IsNull()) {
            read(res.value());
        }
    }

private:
    rapidjson::Value val;
};
