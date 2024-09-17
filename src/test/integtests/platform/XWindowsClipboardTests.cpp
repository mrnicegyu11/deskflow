/*
 * Deskflow -- mouse and keyboard sharing utility
 * Copyright (C) 2012-2016 Symless Ltd.
 * Copyright (C) 2011 Nick Bolton
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file LICENSE that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "platform/XWindowsClipboard.h"

#include "test/shared/undef_x11_macros.h"

#include <gtest/gtest.h>
#include <memory>

// TODO: fix segfault in release mode
#if 0

class XWindowsClipboardTests : public ::testing::Test {
protected:
  void SetUp() override {
    m_display = XOpenDisplay(nullptr);
    int screen = DefaultScreen(m_display);
    Window root = XRootWindow(m_display, screen);

    XSetWindowAttributes attr;
    attr.do_not_propagate_mask = 0;
    attr.override_redirect = True;
    attr.cursor = Cursor();

    m_window = XCreateWindow(
        m_display, root, 0, 0, 1, 1, 0, 0, InputOnly, nullptr, 0, &attr);

    m_clipboard = std::make_unique<XWindowsClipboard>(m_display, m_window, 0);
    m_clipboard->open(0);
    m_clipboard->empty();
  }

  void TearDown() override {
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
  }

  XWindowsClipboard &getClipboard() { return *m_clipboard; }

private:
  Display *m_display;
  Window m_window;
  std::unique_ptr<XWindowsClipboard> m_clipboard;
};

TEST_F(XWindowsClipboardTests, empty_openCalled_returnsTrue) {
  XWindowsClipboard &clipboard = getClipboard();

  bool actual = clipboard.empty();

  EXPECT_EQ(true, actual);
}

TEST_F(XWindowsClipboardTests, empty_singleFormat_hasReturnsFalse) {
  XWindowsClipboard &clipboard = getClipboard();
  clipboard.add(XWindowsClipboard::kText, "deskflow rocks!");

  clipboard.empty();

  bool actual = clipboard.has(XWindowsClipboard::kText);
  EXPECT_FALSE(actual);
}

TEST_F(XWindowsClipboardTests, add_newValue_valueWasStored) {
  XWindowsClipboard &clipboard = getClipboard();

  clipboard.add(IClipboard::kText, "deskflow rocks!");

  String actual = clipboard.get(IClipboard::kText);
  EXPECT_EQ("deskflow rocks!", actual);
}

TEST_F(XWindowsClipboardTests, add_replaceValue_valueWasReplaced) {
  XWindowsClipboard &clipboard = getClipboard();

  clipboard.add(IClipboard::kText, "deskflow rocks!");
  clipboard.add(IClipboard::kText, "maxivista sucks"); // haha, just kidding.

  String actual = clipboard.get(IClipboard::kText);
  EXPECT_EQ("maxivista sucks", actual);
}

TEST_F(XWindowsClipboardTests, close_isOpen_noErrors) {
  XWindowsClipboard &clipboard = getClipboard();

  // clipboard opened in createClipboard()
  clipboard.close();

  // can't assert anything
}

TEST_F(XWindowsClipboardTests, has_withFormatAdded_returnsTrue) {
  XWindowsClipboard &clipboard = getClipboard();
  clipboard.add(IClipboard::kText, "deskflow rocks!");

  bool actual = clipboard.has(IClipboard::kText);

  EXPECT_EQ(true, actual);
}

TEST_F(XWindowsClipboardTests, has_withNoFormats_returnsFalse) {
  XWindowsClipboard &clipboard = getClipboard();

  bool actual = clipboard.has(IClipboard::kText);

  EXPECT_FALSE(actual);
}

TEST_F(XWindowsClipboardTests, get_withNoFormats_returnsEmpty) {
  XWindowsClipboard &clipboard = getClipboard();

  String actual = clipboard.get(IClipboard::kText);

  EXPECT_EQ("", actual);
}

TEST_F(XWindowsClipboardTests, get_withFormatAdded_returnsExpected) {
  XWindowsClipboard &clipboard = getClipboard();
  clipboard.add(IClipboard::kText, "deskflow rocks!");

  String actual = clipboard.get(IClipboard::kText);

  EXPECT_EQ("deskflow rocks!", actual);
}

#endif
