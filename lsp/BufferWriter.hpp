#pragma once

#include <rapidjson/writer.h>

#include <FStr.hpp>
#include <Range.hpp>
#include <vector>

struct BufferWriter {
    struct Object {
        Object(BufferWriter& parent, const char* name) : parent(parent), closed(false) {
            if (name && *name) {
                parent.w.Key(name);
            }
            parent.w.StartObject();
        }

        ~Object() {
            if (!closed) {
                close();
            }
        }

        void close() {
            parent.w.EndObject();
            closed = true;
        }

        template <typename T>
        void writeMember(const char* name, const T& fwd) {
            parent.w.Key(name);
            parent.write(fwd);
        }

        template <typename T>
        void writeMember(const char* name, const std::vector<T>& arr) {
            parent.w.Key(name);
            parent.w.StartArray();
            for (int i = 0; i < arr.size(); ++i) {
                parent.write(arr[i]);
            }
            parent.w.EndArray();
        }

    private:
        Object(const Object&) = delete;
        Object(Object&&) = delete;

        int closed;
        BufferWriter& parent;
    };

    BufferWriter(rapidjson::Writer<rapidjson::StringBuffer>& w) : w(w) {}

    Object beginObject(const char* name = "") { return Object(*this, name); }

    template <typename T, typename std::enable_if<std::is_class<T>::value, bool>::type = true>
    void write(const T& val) {
        val.write(*this);
    }

    template <typename T, typename std::enable_if<std::is_enum<T>::value, bool>::type = true>
    void write(T e) {
        w.Int(static_cast<int>(e));
    }

    template <typename T>
    void write(const std::vector<T>& arr) {
        w.StartArray();
        for (int i = 0; i < arr.size(); ++i) {
            write(arr[i]);
        }
        w.EndArray();
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

    void write(const char* s) {
        w.String(s);
    }

    void write(const Position& pos) {
        w.StartObject();
        w.Key("line");
        w.Int(pos.line);
        w.Key("character");
        w.Int(pos.character);
    }

    void write(const Range& range) {
        w.StartObject();
        w.Key("start");
        write(range.start);
        w.Key("end");
        write(range.end);
    }

private:
    rapidjson::Writer<rapidjson::StringBuffer>& w;
};
