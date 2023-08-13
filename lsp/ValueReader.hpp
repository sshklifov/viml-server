#pragma once

#include <rapidjson/document.h>

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

private:
	rapidjson::Value val;
};
