#include "BSML/Components/Settings/StringSetting.hpp"
#include "logging.hpp"

#include "UnityEngine/UI/Button_ButtonClickedEvent.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

DEFINE_TYPE(BSML, StringSetting);

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace BSML {
    void StringSetting::ctor() {
        genericSetting = GenericSettingWrapper::New_ctor();
        currentValue = "";
    }

    bool StringSetting::get_interactable() {
        return editButton ? editButton->get_interactable() : false;
    }

    void StringSetting::set_interactable(bool value) {
        if (editButton) editButton->set_interactable(value);
    }

    StringW StringSetting::get_text() {
        return currentValue;
    }

    void StringSetting::set_text(StringW value) {
        currentValue = value;
        if (text) 
            text->set_text(formatter ? formatter(value) : value);
    }

    void StringSetting::Start() {
        Setup();
        modalKeyboard->onEnter = std::bind(&StringSetting::EnterPressed, this, std::placeholders::_1);

        editButton->set_onClick(Button::ButtonClickedEvent::New_ctor());
        std::function<void()> fun = std::bind(&StringSetting::EditButtonPressed, this);
        auto delegate = il2cpp_utils::MakeDelegate<Events::UnityAction*>(classof(Events::UnityAction*), fun);
        editButton->get_onClick()->AddListener(delegate);
    }

    void StringSetting::Setup() {
        modalKeyboard->clearOnOpen = false;
        ReceiveValue();
    }

    void StringSetting::EditButtonPressed() {
        modalKeyboard->modalView->Show();
        modalKeyboard->SetText(get_text());
    }

    void StringSetting::EnterPressed(StringW value) {
        set_text(value);
        if (genericSetting) {
            auto val = get_text();
            genericSetting->OnChange(val);
            if (genericSetting->applyOnChange) ApplyValue();
        }
    }

    void StringSetting::ApplyValue() {
        if (genericSetting) 
            genericSetting->SetValue(get_text());
    }

    void StringSetting::ReceiveValue() {
        if (genericSetting) 
            set_text(genericSetting->GetValue<Il2CppString*>());
    }

}