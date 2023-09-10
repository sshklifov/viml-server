#pragma once

#include <rapidjson/document.h>

#include <FStr.hpp>
#include <Range.hpp>
#include <Vector.hpp>

#include <optional>

struct ValueReader {
    struct MemberScope {
        MemberScope(ValueReader& parent, const char* name) : parent(parent), origVal(parent.val) {
            rapidjson::Value::ConstMemberIterator it = parent.val->FindMember(name);
            assert(it != parent.val->MemberEnd());
            if (it != parent.val->MemberEnd()) {
                parent.val = &it->value;
            }
        }

        ~MemberScope() {
            parent.val = origVal;
        }

        ValueReader& parent;
        const rapidjson::Value* origVal;
        bool valid;
    };

    explicit ValueReader(const rapidjson::Value& val) : val(&val) {}

    const rapidjson::Value& getValue() const { return *val; }

    template <typename T>
    void readMember(const char* name, T& res) {
        MemberScope scope = beginMember(name);
        read(res);
    }

    template <typename T>
    void readMember(const char* name, std::optional<T>& res) {
        if (val->HasMember(name)) {
            MemberScope scope = beginMember(name);
            read(res.value());
        }
    }

private:
    template <typename T, typename std::enable_if<std::is_class<T>::value, bool>::type = true>
    void read(T& res) {
        res.read(*this);
    }

    void read(int& res) {
        assert(val->IsInt());
        if (val->IsInt()) {
            res = val->GetInt();
        }
    }

    void read(bool& res) {
        assert(val->IsBool());
        if (val->IsBool()) {
            res = val->GetBool();
        }
    }

    void read(FStr& res) {
        assert(val->IsString());
        if (val->IsString()) {
            res = val->GetString();
        }
    }

    void read(Position& pos) {
        readMember("line", pos.line);
        readMember("character", pos.character);
    }

    void read(Range& range) {
        readMember("start", range.start);
        readMember("end", range.end);
    }

    template <typename T>
    void read(Vector<T>& res) {
        if (!val->IsArray()) {
            assert(false);
            return;
        }
        const rapidjson::Value& arr = *val;
        res.resize(arr.Size());
        for (int i = 0; i < arr.Size(); ++i) {
            val = &arr[i];
            read(res[i]);
        }
        val = &arr;
    }

    MemberScope beginMember(const char* name) {
        return MemberScope(*this, name);
    }

    const rapidjson::Value* val;
};
