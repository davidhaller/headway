/*
 * Copyright (C) 2017 David Haller <davidhaller@mailbox.org>
 *
 * headway is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * headway is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <glibmm.h>
#include <giomm.h>
#include <gtkmm.h>
#include <goocanvasmm.h>
#include <sigc++/sigc++.h>

#include "world.hpp"

namespace headway
{
    /*! \brief Main window for display and setting parameters.
     */

    class window : public Gtk::ApplicationWindow
    {
    public:
        explicit window();
        virtual ~window() noexcept {}

        /*! \brief Shows the about dialog.
         */

        void about() noexcept;

        /*! \brief Sets window to displaying a specific world.
         *
         * \param biotope std::shared_ptr to a world object.
         * \param title Text to display in the window titlebar, usually the filename.
         */

        void display(std::shared_ptr<headway::world> biotope, const std::string& title = "") noexcept;

    protected:
        /*! \brief Throws away canvas and redraws everything.
         */

        void redraw() noexcept;

        /*! \brief Refreshes the canvas to reflect the changes in the biotope.
         *
         * \returns true if drawing was successful, false on error.
         */

        bool refresh_all() noexcept;

        /*! \brief Refreshes one single cell in the canvas.
         */

        void refresh_cell(unsigned int x, unsigned int y) noexcept;

        /*! \brief Updates information on controlbar.
         */

        void update() noexcept;

    private:
        std::shared_ptr<headway::world> biotope;
        std::mutex worker_mtx;
        std::atomic<bool> worker_active = ATOMIC_VAR_INIT(false);
        std::chrono::milliseconds worker_naptime;
        sigc::connection timer;

        Glib::RefPtr<Gio::SimpleAction> new_action;
        Glib::RefPtr<Gio::SimpleAction> load_action;
        Glib::RefPtr<Gio::SimpleAction> save_action;
        Glib::RefPtr<Gio::SimpleAction> random_action;
        Glib::RefPtr<Gio::SimpleAction> run_action;
        Glib::RefPtr<Gio::SimpleAction> next_action;

        Glib::RefPtr<Gtk::FileFilter> filter_xml;
        Glib::RefPtr<Gtk::FileFilter> filter_json;
        Glib::RefPtr<Gtk::FileFilter> filter_none;

        Glib::RefPtr<Gtk::Adjustment> width_adjustment;
        Glib::RefPtr<Gtk::Adjustment> height_adjustment;

        void create_new_world();
        void open_file();
        void save_file();
        void create_random_cells();
        void toggle_simulation();
        void next_generation();
        void apply_speed();
        void mouse_click(double x, double y);

        Goocanvas::Canvas canvas;
        std::unique_ptr<headway::matrix<Glib::RefPtr<Goocanvas::RectModel>>> shapes;

        Gtk::Box layout;
        Gtk::HeaderBar headerbar;
        Gtk::Separator separator;
        Gtk::Box controlbar;
        Gtk::ButtonBox controls_left;
        Gtk::ButtonBox controls_right;

        Gtk::Button new_button;
        Gtk::Button load_button;
        Gtk::Button save_button;

        Gtk::Button random_button;
        Gtk::ToggleButton run_button;
        Gtk::Button next_button;

        Gtk::Label count_label;
        Gtk::Label generations_label;
        Gtk::Scale speed_scale;
    };
}
