#pragma once

#include <rapidjson/writer.h>

#include <FStr.hpp>
#include <vector>

struct BufferWriter {
    struct Object {
        Object(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* name) : w(w) {
            w.Key(name);
            w.StartObject();
        }

        void close() {
            w.EndObject();
        }

        Object(const Object&) = delete;
        Object(Object&&) = delete;

    private:
        rapidjson::Writer<rapidjson::StringBuffer>& w;
    };

    struct ScopedObject : private Object {
        ScopedObject(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* name) : Object(w, name) {}

        ~ScopedObject() {
            close();
        }
    };

    BufferWriter(rapidjson::Writer<rapidjson::StringBuffer>& w) : w(w) {}

    ScopedObject beginScopedObject(const char* name = "") { return ScopedObject(w, name); }

    Object beginObject(const char* name = "") { return Object(w, name); }

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
