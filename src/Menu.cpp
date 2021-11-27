//
// Created by kbar on 30.10.2021.
//

#include "Menu.h"

namespace ribbon {
    Menu::Menu(Adafruit_SH1107 &_display, Encoder &_encoder) : display(_display), encoder(_encoder) {}

    std::shared_ptr<MenuPage> Menu::addPage(const std::string &name) {
        std::shared_ptr<MenuPage> page = std::make_shared<MenuPage>(name);
        menuPages.emplace_back(page);
        return page;
    }

    std::shared_ptr<MenuPage> Menu::getPageByName(const std::string &pageName)
    {
        for (auto &page : menuPages) {
            if (page->pageName == pageName)
                return page;
        }
    }

    void Menu::encoderService() {
        const int32_t newPos = encoder.read();
        if ((newPos / 4) != previousEncoderValue) {
            encoderValueChanged(newPos / 4);
            previousEncoderValue = newPos / 4;
        }
    }

    void Menu::encoderValueChanged(const int32_t newEncoderValue) {}

    void Menu::encoderButtonClicked() {}

    void Menu::renderPagebar() {

        display.fillRect(0, 112, 128, 10, SH110X_WHITE);
        display.setTextColor(SH110X_BLACK);
        display.setCursor(2, 114);
        display.printf("%d/%d %s(%2.0u)", currentPageIndex + 1, menuPages.size(),
                       getCurrentPage() ? getCurrentPage()->pageName.c_str() : "no pages", millis() / 100);
        display.display();
    }

    void Menu::renderItems() {
        if (getCurrentPage() == nullptr || getCurrentPage()->itemsOnPage.size() == 0) {
            display.clearDisplay();
            display.fillRect(32, 32, 64, 10, SH110X_WHITE);
            display.setTextColor(SH110X_BLACK);
            display.setCursor(32, 32);
            display.printf("No items");
        }
    }
    void Menu::begin() {
        areWeValid = true;
        addPage("Unknown");
    }

    void Menu::update() {
        if (areWeValid == false)
            return;

        if (renderTimer > 10) {
            renderTimer = 0;
            encoderService();
            renderItems();
            renderPagebar();
        }
    }
}