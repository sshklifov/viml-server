#pragma once

#include <rapidjson/writer.h>

#include <string>
#include <vector>

struct ObjectScope {
    ObjectScope(rapidjson::Writer<rapidjson::StringBuffer>& w, const char* name = 0) : w(w) {
        if (name && *name) {
            w.Key(name);
        }
        w.StartObject();
        closed = false;
    }

    ObjectScope(const ObjectScope&) = delete;
    ObjectScope(ObjectScope&&) = delete;

    ~ObjectScope() {
        if (!closed) {
            w.EndObject();
        }
    }

    void close() {
        assert(!closed);
        closed = true;
        w.EndObject();
    }

private:
    rapidjson::Writer<rapidjson::StringBuffer>& w;
    int closed;
};

struct Writer {
    Writer(rapidjson::Writer<rapidjson::StringBuffer>& w) : w(w) {}

    ObjectScope newObject(const char* name = nullptr) { return ObjectScope(w, name); }

    template <typename T>
    Writer& add(const char* name, const T& fwd) {
        w.Key(name);
        setKey(fwd);
        return (*this);
    }

    template <typename T>
    Writer& add(const char* name, const std::vector<T>& arr) {
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

    void setKey(const std::string& str) {
        w.String(str.c_str());
    }

    void setKey(bool flag) {
        w.Bool(flag);
    }

    void setKey(int integer) {
        w.Int(integer);
    }

    template <typename T>
    void setKey(const T& val) {
        w.Null();
        abort();
    }

private:
    rapidjson::Writer<rapidjson::StringBuffer>& w;
};
