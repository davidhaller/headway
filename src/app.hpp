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

#include <glibmm.h>
#include <giomm.h>
#include <gtkmm.h>

namespace headway
{
    constexpr auto name = "Headway";
    constexpr auto version = "3.2.1";
    constexpr auto author = "David Haller <davidhaller@mailbox.org>";
    constexpr auto copyright = "Copyright © 2017 David Haller";
    constexpr auto description = "Conway's Game of Life implementation in C++";
    constexpr auto website = "https://github.com/davidhaller/headway";
    constexpr auto license = Gtk::LICENSE_GPL_3_0;

    /*! \brief Application manager ensuring correct initialization.
     */

    class app : public Gtk::Application
    {
    public:

        /*! \brief Factory method to create a new application instance.
         */

        static Glib::RefPtr<headway::app> create();

        /*! \brief Shows the about dialog of the application.
         *
         * \param parent Parent window.
         */

        static void show_about(Gtk::Window& parent) noexcept;

        /*! \brief Shows an error dialog with an message.
         *
         * \param parent Parent window.
         * \param title Title of the error message.
         * \param message Detailed description of the error.
         */

        static void show_error(Gtk::Window& parent, const Glib::ustring& title,
                               const Glib::ustring& message = "") noexcept;

        /*! \brief Connects a widget with an action.
         *
         * This is a workaround for a gtkmm limitation as Gtk::Button and other
         * widgets do not implement the Gtk::Actionable interface, so the method
         * set_action_name is not available.
         *
         * Developers of gtkmm won't fix this in the near future,
         * as it would break ABI. In the meantime, we must call
         * the native C function gtk_actionable_set_action_name.
         *
         * See also: https://bugzilla.gnome.org/show_bug.cgi?id=759756
         *
         * \param actionable Any GObject that implements the Actionable interface.
         * \param action_name Unique action name as a string.
         */

        static inline void set_action_name(Glib::Object& actionable, const Glib::ustring& action_name) noexcept
        {
            gtk_actionable_set_action_name(GTK_ACTIONABLE(actionable.gobj()), action_name.c_str());
        }

    protected:
        explicit app();
        virtual ~app() noexcept {};

        void on_startup() override;
        void on_activate() override;
        void on_open(const Gtk::Application::type_vec_files& files, const Glib::ustring& hint) override;

    private:
        void on_window_hide(Gtk::Window* window) noexcept;
        void create_window(Glib::RefPtr<Gio::File> file = Glib::RefPtr<Gio::File>());
    };
}
