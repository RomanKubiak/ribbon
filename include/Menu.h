//
// Created by kbar on 30.10.2021.
//

#ifndef RIBBONT_MENU_H
#define RIBBONT_MENU_H


#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <Encoder.h>
#include <memory>

#include "../lib/Adafruit_SH110x/Adafruit_SH110X.h"

namespace ribbon {
    enum ItemType {
        Numeric,
        String,
        Boolean,
        Unknown
    };

    struct MenuItem {
        std::string itemName = "Unknown";
        ItemType itemType = Numeric;
        std::function<bool(int)> valueChanged;
        std::vector<std::string> possibleStringValues = { "" };
        double numericValue = .0;
        bool booleanValue = false;
        std::uint32_t currentStringValue = 0;
        bool active = true;
        std::function<bool(void)> isActive;
        bool operator ==(const MenuItem &rhs) const noexcept
        {
            return itemName == rhs.itemName;
        }

        bool operator ==(const std::string &name) const noexcept
        {
            return itemName == name;
        }
        MenuItem(const std::string &_itemName) : itemName(_itemName) {}
    };

    struct MenuPage {
        std::string pageName = "Unknown";
        std::vector<std::shared_ptr<MenuItem>> itemsOnPage;
        std::function<void (int)> pageChanged;
        std::uint32_t activeItemOnPage = 0;
        std::shared_ptr <MenuItem> addItem(const std::string &name) {
            std::shared_ptr<MenuItem> menuItem = std::make_shared<MenuItem>(name);
            itemsOnPage.emplace_back(menuItem);
            return menuItem;
        }

        bool operator ==(const MenuPage &rhs) const noexcept
        {
            return pageName == rhs.pageName;
        }
        bool operator ==(const std::string &name) const noexcept
        {
            return pageName == name;
        }
        MenuPage(const std::string &_pageName) : pageName(_pageName) {}
    };

    class Menu {
    public:
        Menu(Adafruit_SH1107 &_display, Encoder &_encoder);
        std::shared_ptr<MenuPage> addPage(const std::string &name);
        std::shared_ptr<MenuPage> getCurrentPage() { return menuPages[currentPageIndex]; }
        std::shared_ptr<MenuPage> getPageByName(const std::string &pageName);
        void begin();
        void update();

    private:
        void encoderService();
        void encoderValueChanged(const int32_t newEncoderValue);
        void encoderButtonClicked();

        void renderPagebar();
        void renderItems();
        Adafruit_SH1107 &display;
        Encoder &encoder;
        std::vector <std::shared_ptr<MenuPage> > menuPages;
        uint32_t currentPageIndex = 0;
        int32_t previousEncoderValue = 0;
        bool areWeValid = false;
        elapsedMillis renderTimer;
    };
}

#endif //RIBBONT_MENU_H
