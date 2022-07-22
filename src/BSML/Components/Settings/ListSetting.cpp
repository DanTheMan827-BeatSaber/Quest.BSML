#include "BSML/Components/Settings/ListSetting.hpp"

DEFINE_TYPE(BSML, ListSetting);

namespace BSML {
    void ListSetting::ctor() {
        IncDecSetting::construct();
        values = List<Il2CppObject*>::New_ctor();
        index = 0;
    }

    void ListSetting::Setup() {
        ReceiveValue();
        if (values.size() > 0)
            UpdateState();
    }

    void ListSetting::IncButtonPressed() {
        index++;
        EitherPressed();
    }

    void ListSetting::DecButtonPressed() {
        index--;
        EitherPressed();
    }

    void ListSetting::EitherPressed() {
        UpdateState();
        ApplyValue();
    }

    void ListSetting::ReceiveValue() {
        if (!associatedValue) return;
        set_Value(associatedValue->GetValue<Il2CppObject*>());
    }

    void ListSetting::ApplyValue() {
        if (!associatedValue) return;
        associatedValue->SetValue(get_Value());
    }

    void ListSetting::ValidateRange() {
        if (index >= values.size())
            index = values.size() - 1;
        if (index < 0)
            index = 0;
    }

    void ListSetting::UpdateState() {
        if (values.size() > 0) {
            set_enableDec(index > 0);
            set_enableInc(index < values.size() - 1);
            StringW text; 
            if (formatter)
                text = formatter(values[index]);
            else {
                auto value = values[index];
                if (value) {
                    text = value->ToString();
                } else {
                    text = "NULL";
                }
            }
            set_text(text);
        } else {
            set_enableDec(false);
            set_enableInc(false);
            set_text("No values");
        }
    }

    Il2CppObject* ListSetting::get_Value() {
        ValidateRange();
        if (values.size() == 0) return nullptr;
        return values[index];
    }

    void ListSetting::set_Value(Il2CppObject* value) {
        index = 0;
        for (auto& v : values) {
            // if both are the same, or v has a value and Equals the value
            if ((v == value) || (v && v->Equals(value)))
                break;
            index++;
        }

        if (index == values.size())
            index = 0;
        
        UpdateState();
    }
}