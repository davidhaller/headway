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

#include "app.hpp"
#include "file.hpp"
#include "window.hpp"

using namespace headway;

Glib::RefPtr<headway::app> app::create()
{
    return Glib::RefPtr<headway::app>(new app());
}

app::app() : Gtk::Application("davidhaller.headway", Gio::ApplicationFlags::APPLICATION_HANDLES_OPEN)
{
    Glib::set_application_name(headway::name);
}

void app::on_startup()
{
    Gtk::Application::on_startup();

    add_action("new", sigc::mem_fun(*this, &app::activate));
    add_action("quit", sigc::mem_fun(*this, &app::quit));

    set_accel_for_action("app.new", "<Control>N");
    set_accel_for_action("win.load", "<Control>O");
    set_accel_for_action("win.save", "<Control>S");
    set_accel_for_action("win.close", "<Control>W");
    set_accel_for_action("app.quit", "<Control>Q");

    auto menu = Gio::Menu::create();
    menu->append("New Window", "app.new");
    menu->append("About", "win.about");
    menu->append("Quit", "app.quit");
    set_app_menu(menu);
}

void app::on_activate()
{
    Gtk::Application::on_activate();
    create_window();
}

void app::on_open(const Gtk::Application::type_vec_files& files, const Glib::ustring& hint)
{
    Gtk::Application::on_open(files, hint);

    for (const Glib::RefPtr<Gio::File>& file : files)
        create_window(file);
}

void app::on_window_hide(Gtk::Window* window) noexcept
{
    delete window;
}

void app::create_window(const Glib::RefPtr<Gio::File> file)
{
    auto window = new headway::window();
    add_window(*window);

    window->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &app::on_window_hide), window));
    window->show_all();

    if (!file)
        return;

    try
    {
        const std::string path = file->get_path();
        std::shared_ptr<headway::file> input = headway::world::load(path);
        std::shared_ptr<headway::world> biotope = std::make_shared<headway::world>(input);

        window->display(biotope, path);
    }

    catch (std::exception& ex)
    {
        show_error(*window, "Error reading file.", ex.what());
        window->close();
    }
}

void app::show_about(Gtk::Window& parent) noexcept
{
    Gtk::AboutDialog dialog;
    dialog.set_transient_for(parent);

    dialog.set_program_name(headway::name);
    dialog.set_version(headway::version);
    dialog.set_authors({headway::author});
    dialog.set_copyright(headway::copyright);
    dialog.set_comments(headway::description);
    dialog.set_license_type(headway::license);
    dialog.set_website(headway::website);

    dialog.run();
}

void app::show_error(Gtk::Window& parent, const Glib::ustring& title, const Glib::ustring& message) noexcept
{
    Gtk::MessageDialog dialog(title, false, Gtk::MessageType::MESSAGE_ERROR);
    dialog.set_transient_for(parent);
    dialog.set_secondary_text("Reason: " + message);
    dialog.run();
}
