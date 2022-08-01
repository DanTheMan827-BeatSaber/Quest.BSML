#include "BSML/Tags/ScrollableContainerTag.hpp"
#include "Helpers/getters.hpp"

#include "BSML/Components/ScrollableContainer.hpp"
#include "BSML/Components/ExternalComponents.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Mask.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/Touchable.hpp"
#include "HMUI/EventSystemListener.hpp"

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace BSML {
    static BSMLTagParser<ScrollableContainerTag> scrollableContainerTagParser({"scrollable-container"});

    UnityEngine::GameObject* ScrollableContainerTag::CreateObject(UnityEngine::Transform* parent) const {
        auto go = GameObject::New_ctor("BSMLScrollableContainer");
        go->SetActive(false);

        auto transform = go->AddComponent<RectTransform*>();
        transform->SetParent(parent, false);
        transform->set_localPosition({0, 0, 0});
        transform->set_anchoredPosition({0, 0});
        transform->set_anchorMin({0, 0});
        transform->set_anchorMax({0, 0});
        transform->set_sizeDelta({0, 0});

        auto vpgo = GameObject::New_ctor("Viewport");
        auto viewport = vpgo->AddComponent<RectTransform*>();
        viewport->SetParent(transform, false);
        viewport->set_localPosition({0, 0, 0});
        viewport->set_anchoredPosition({0, 0});
        viewport->set_anchorMin({0, 0});
        viewport->set_anchorMax({0, 0});
        viewport->set_sizeDelta({0, 0});

        auto vpMask = vpgo->AddComponent<Mask*>();
        auto vpImage = vpgo->AddComponent<HMUI::ImageView*>();
        vpMask->set_showMaskGraphic(false);
        vpImage->set_color({1, 1, 1, 1});
        // TODO: white pixel image default
        //vpImage->set_sprite(WhitePixel);
        vpImage->set_material(Helpers::GetUINoGlowMat());

        auto contentGo = GameObject::New_ctor("Content Wrapper");
        auto content = contentGo->AddComponent<RectTransform*>();
        content->SetParent(viewport, false);
        content->set_localPosition({0, 0, 0});
        content->set_anchoredPosition({0, 0});
        content->set_anchorMin({0, 1});
        content->set_anchorMax({1, 1});
        content->set_sizeDelta({0.5, 1});

        auto contentFitter = contentGo->AddComponent<ContentSizeFitter*>();
        contentFitter->set_horizontalFit(ContentSizeFitter::FitMode::Unconstrained);
        contentFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

        auto layout = contentGo->AddComponent<VerticalLayoutGroup*>();
        layout->set_childControlHeight(false);
        layout->set_childForceExpandHeight(false);
        layout->set_childForceExpandWidth(false);

        go->AddComponent<HMUI::Touchable*>(); // Required by EventSystemListener
        go->AddComponent<HMUI::EventSystemListener*>(); // Required by ScrollView
        auto scrollView = go->AddComponent<ScrollableContainer*>();
        scrollView->contentRectTransform = content;
        scrollView->viewport = viewport;
        scrollView->platformHelper = Helpers::GetIVRPlatformHelper();

        auto externalComponents = contentGo->AddComponent<ExternalComponents*>();
        externalComponents->Add(scrollView);

        go->SetActive(true);
        return contentGo;
    }
}