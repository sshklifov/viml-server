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
    BufferWriter& writeMember(const char* name, const T& fwd) {
        w.Key(name);
        write(fwd);
        return (*this);
    }

    template <typename T>
    BufferWriter& writeMember(const char* name, const std::vector<T>& arr) {
        w.Key(name);
        w.StartArray();
        for (int i = 0; i < arr.size(); ++i) {
            write(arr[i]);
        }
        w.EndArray();
        return (*this);
    }

private:
    template <typename T, typename std::enable_if<std::is_class<T>::value, bool>::type = true>
    void write(const T& val) {
        val.write(*this);
    }

    template <typename T, typename std::enable_if<std::is_enum<T>::value, bool>::type = true>
    void write(T e) {
        w.Int(static_cast<int>(e));
    }

    void write(int integer) {
        w.Int(integer);
    }

    void write(bool flag) {
        w.Bool(flag);
    }

    void write(const FStr& fstr) {
        w.String(fstr.str());
    }

    rapidjson::Writer<rapidjson::StringBuffer>& w;
};
