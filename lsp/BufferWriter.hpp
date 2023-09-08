#pragma once

#include <rapidjson/writer.h>

#include <FStr.hpp>
#include <vector>

struct BufferWriter {
    struct ObjectScope {
        ObjectScope(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* name = 0) : w(w) {
            if (name && *name) {
                w.Key(name);
            }
            w.StartObject();
        }

        ~ObjectScope() {
            w.EndObject();
        }

        ObjectScope(const ObjectScope&) = delete;
        ObjectScope(ObjectScope&&) = delete;

    private:
        rapidjson::Writer<rapidjson::StringBuffer>& w;
    };

    BufferWriter(rapidjson::Writer<rapidjson::StringBuffer>& w) : w(w) {}

    ObjectScope beginObject(const char* name = nullptr) { return ObjectScope(w, name); }

    template <typename T>
    BufferWriter& add(const char* name, const T& fwd) {
        w.Key(name);
        setKey(fwd);
        return (*this);
    }

    template <typename T>
    BufferWriter& add(const char* name, const std::vector<T>& arr) {
        w.Key(name);
        w.StartArray();
        for (int i = 0; i < arr.size(); ++i) {
            setKey(arr[i]);
        }
        w.EndArray();
        return (*this);
    }

    void setKey(const char* str) {
        w.String(str);
    }

    void setKey(const FStr& fstr) {
        w.String(fstr.str());
    }

    void setKey(bool flag) {
        w.Bool(flag);
    }

    void setKey(int integer) {
        w.Int(integer);
    }

    template <typename T>
    void setKey(const T& val) {
        static_assert(!std::is_enum<T>::value, "enum should be cast to int");
        w.Null();
        abort();
    }

private:
    rapidjson::Writer<rapidjson::StringBuffer>& w;
};
