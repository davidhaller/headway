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

window::window() : layout(Gtk::Orientation::ORIENTATION_VERTICAL)
{
    set_size_request(700, 600);

    new_action = add_action("new", sigc::mem_fun(*this, &window::create_new_world));
    headway::app::set_action_name(new_button, "win.new");
    new_button.set_label("New");
    new_button.set_tooltip_text("Create a new world");

    load_action = add_action("load", sigc::mem_fun(*this, &window::open_file));
    headway::app::set_action_name(load_button, "win.load");
    load_button.set_tooltip_text("Load world from file");
    load_button.set_image_from_icon_name("document-open-symbolic");

    save_action = add_action("save", sigc::mem_fun(*this, &window::save_file));
    save_action->set_enabled(false);
    headway::app::set_action_name(save_button, "win.save");
    save_button.set_tooltip_text("Save world to file");
    save_button.set_image_from_icon_name("document-save-symbolic");

    random_action = add_action("random", sigc::mem_fun(*this, &window::create_random_cells));
    random_action->set_enabled(false);
    headway::app::set_action_name(random_button, "win.random");
    random_button.set_label("Random");
    random_button.set_tooltip_text("Generate random cells");

    run_action = add_action("run", sigc::mem_fun(*this, &window::toggle_simulation));
    run_action->set_enabled(false);
    headway::app::set_action_name(run_button, "win.run");
    run_button.set_active(false);
    run_button.set_tooltip_text("Start or stop the simulation");
    run_button.set_image_from_icon_name("media-playback-start-symbolic");

    next_action = add_action("next", sigc::mem_fun(*this, &window::next_generation));
    next_action->set_enabled(false);
    headway::app::set_action_name(next_button, "win.next");
    next_button.set_tooltip_text("Calculate and display the next generation");
    next_button.set_image_from_icon_name("media-skip-forward-symbolic");

    add_action("about", sigc::mem_fun(*this, &window::about));
    add_action("close", sigc::mem_fun(*this, &window::close));

    filter_json = Gtk::FileFilter::create();
    filter_json->set_name("JSON files");
    filter_json->add_pattern("*.json");

    filter_xml = Gtk::FileFilter::create();
    filter_xml->set_name("XML files");
    filter_xml->add_pattern("*.xml");

    filter_none = Gtk::FileFilter::create();
    filter_none->set_name("All files");
    filter_none->add_pattern("*");

    width_adjustment = Gtk::Adjustment::create(30, 1, 100, 1);
    height_adjustment = Gtk::Adjustment::create(30, 1, 100, 1);

    canvas.property_expand() = true;
    canvas.property_sensitive() = false;
    canvas.property_background_color() = "white";

    canvas.signal_button_release_event().connect([this] (GdkEventButton* event)
    {
        mouse_click(event->x, event->y);
        return false;
    });

    signal_event_after().connect([this] (GdkEvent* event)
    {
        if (event->type == GDK_EXPOSE or event->type == GDK_CONFIGURE)
            redraw();
    });

    layout.add(canvas);
    layout.add(separator);

    headerbar.set_show_close_button(true);
    headerbar.set_title(headway::name);
    set_titlebar(headerbar);

    headerbar.pack_start(new_button);
    headerbar.pack_end(save_button);
    headerbar.pack_end(load_button);

    controls_left.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_START);
    controls_left.set_spacing(5);
    controls_left.set_margin_left(5);
    controls_left.pack_start(random_button);
    controls_left.pack_start(run_button);
    controls_left.pack_start(next_button);

    auto speed_adjustment = Gtk::Adjustment::create(20, 1, 100, 1);
    speed_scale.set_adjustment(speed_adjustment);
    speed_scale.set_round_digits(0);
    speed_scale.set_digits(0);
    speed_scale.set_sensitive(false);
    speed_scale.signal_value_changed().connect(sigc::mem_fun(*this, &window::apply_speed));
    apply_speed(); // set initial value of worker_naptime

    count_label.set_text("0 cells");
    generations_label.set_text("0 generations");

    controls_right.set_layout(Gtk::ButtonBoxStyle::BUTTONBOX_END);
    controls_right.set_spacing(5);
    controls_right.pack_end(count_label);
    controls_right.pack_end(generations_label);
    controls_right.pack_end(speed_scale);

    controlbar.pack_start(controls_left);
    controlbar.pack_end(controls_right);
    layout.add(controlbar);

    add(layout);
}

void window::about() noexcept
{
    headway::app::show_about(*this);
}

void window::display(std::shared_ptr<headway::world> biotope, const std::string& title) noexcept
{
    this->biotope = biotope;
    headerbar.set_subtitle(title);

    save_action->set_enabled(true);
    random_action->set_enabled(true);
    run_action->set_enabled(true);
    next_action->set_enabled(true);
    canvas.set_sensitive(true);
    speed_scale.set_sensitive(true);

    redraw();
}

void window::redraw() noexcept
{
    if (biotope == nullptr)
        return;

    const unsigned int width = biotope->width();
    const unsigned int height = biotope->height();
    shapes = std::make_unique<headway::matrix<Glib::RefPtr<Goocanvas::RectModel>>>(width, height);

    auto surface = Goocanvas::GroupModel::create();
    canvas.set_root_item_model(surface);

    const double box_width = static_cast<double>(canvas.get_width()) / width;
    const double box_height = static_cast<double>(canvas.get_height()) / height;

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            auto rect = Goocanvas::RectModel::create(box_width * x + 1, box_height * y + 1, box_width - 3, box_height - 3);

            surface->add_child(rect);
            shapes->set(x, y, rect);

            rect->property_line_width() = 0.5;
            rect->property_stroke_color() = "gray";
        }
    }

    int size_x, size_y;
    get_size(size_x, size_y);

    canvas.property_x2() = size_x;
    canvas.property_y2() = size_y;

    refresh_all();
}

bool window::refresh_all() noexcept
{
    if (biotope == nullptr)
        return false;

    canvas.set_visible(false);

    const unsigned int width = biotope->width();
    const unsigned int height = biotope->height();

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            refresh_cell(x, y);
        }
    }

    canvas.set_visible(true);

    update();
    return true; // Glib::signal_timeout() needs this to continue execution
}

void window::refresh_cell(unsigned int x, unsigned int y) noexcept
{
    auto rect = shapes->get(x, y);

    if (biotope->is_alive(x, y))
        rect->property_fill_color() = "black";
    else
        rect->property_fill_color() = "white";
}

void window::update() noexcept
{
    const unsigned long count = biotope->count();
    const unsigned long generations = biotope->generations();

    count_label.set_text(std::to_string(count) + (count == 1 ? " cell" : " cells"));
    generations_label.set_text(std::to_string(generations) + (generations == 1 ? " generation" : " generations"));
}

void window::create_new_world()
{
    Gtk::Label width_label("Width: ");
    Gtk::Label height_label("Height: ");

    Gtk::SpinButton width_spin(width_adjustment);
    Gtk::SpinButton height_spin(height_adjustment);

    Gtk::Dialog dialog("Options", *this, Gtk::DialogFlags::DIALOG_USE_HEADER_BAR);
    dialog.set_transient_for(*this);
    dialog.set_size_request(400, 200);
    dialog.set_resizable(false);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_APPLY);

    Gtk::Grid grid;
    grid.attach(width_label, 0, 0, 1, 1);
    grid.attach(width_spin, 1, 0, 1, 1);
    grid.attach(height_label, 0, 1, 1, 1);
    grid.attach(height_spin, 1, 1, 1, 1);
    grid.set_column_spacing(3);
    grid.show_all();

    Gtk::Box* content = dialog.get_content_area();
    content->add(grid);
    content->set_valign(Gtk::Align::ALIGN_CENTER);
    content->set_halign(Gtk::Align::ALIGN_CENTER);

    if (dialog.run() == Gtk::RESPONSE_APPLY)
    {
        const int width = width_spin.get_value_as_int();
        const int height = height_spin.get_value_as_int();

        std::shared_ptr<headway::world> biotope = std::make_shared<headway::world>(width, height);
        display(biotope);
    }
}

void window::open_file()
{
    Gtk::FileChooserDialog dialog(*this, "Select file to load", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    dialog.add_filter(filter_json);
    dialog.add_filter(filter_xml);
    dialog.add_filter(filter_none);

    if (dialog.run() == Gtk::RESPONSE_OK)
    {
        const std::string filename = dialog.get_filename();

        try
        {
            std::shared_ptr<headway::file> input = headway::world::load(filename);
            std::shared_ptr<headway::world> biotope = std::make_shared<headway::world>(input);

            display(biotope, filename);
        }

        catch (std::exception& ex)
        {
            headway::app::show_error(dialog, "Error reading file.", ex.what());
        }
    }
}

void window::save_file()
{
    Gtk::FileChooserDialog dialog(*this, "Select save destination", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.set_do_overwrite_confirmation(true);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

    dialog.add_filter(filter_json);
    dialog.add_filter(filter_xml);
    dialog.add_filter(filter_none);

    if (dialog.run() == Gtk::RESPONSE_OK)
    {
        const std::string filename = dialog.get_filename();

        try
        {
            biotope->save(filename);
            headerbar.set_subtitle(filename);
        }

        catch (std::exception& ex)
        {
            headway::app::show_error(dialog, "Error saving file.", ex.what());
        }
    }
}

void window::create_random_cells()
{
    const unsigned long space = biotope->width() * biotope->height() - biotope->count();
    auto random_adjustment = Gtk::Adjustment::create(space / 2, 1, space, 10);
    Gtk::SpinButton random_spin(random_adjustment);
    Gtk::Label random_label("Cells: ");

    Gtk::Dialog dialog("Random", *this, Gtk::DialogFlags::DIALOG_USE_HEADER_BAR);
    dialog.set_transient_for(*this);
    dialog.set_size_request(400, 150);
    dialog.set_resizable(false);

    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::EXECUTE, Gtk::RESPONSE_OK);

    Gtk::Grid grid;
    grid.attach(random_label, 0, 0, 1, 1);
    grid.attach(random_spin, 1, 0, 1, 1);
    grid.set_column_spacing(3);
    grid.show_all();

    Gtk::Box* content = dialog.get_content_area();
    content->add(grid);
    content->set_valign(Gtk::Align::ALIGN_CENTER);
    content->set_halign(Gtk::Align::ALIGN_CENTER);

    if (dialog.run() == Gtk::RESPONSE_OK)
    {
        const int cells = random_spin.get_value_as_int();
        biotope->random(static_cast<unsigned long>(cells));
        refresh_all();
    }
}

void window::toggle_simulation()
{
    if (run_button.get_active())
    {
        worker_active = true;
        std::thread worker([&]()
        {
            std::lock_guard<std::mutex> lock(worker_mtx);

            while (worker_active)
            {
                biotope->next();
                std::this_thread::sleep_for(worker_naptime);
            }
        });
        worker.detach();
        timer = Glib::signal_timeout().connect(sigc::mem_fun(*this, &window::refresh_all), 100);

        new_action->set_enabled(false);
        load_action->set_enabled(false);
        save_action->set_enabled(false);
        next_action->set_enabled(false);
        canvas.set_sensitive(false);
    }

    else
    {
        worker_active = false;
        std::lock_guard<std::mutex> lock(worker_mtx); // wait until worker has finished

        timer.disconnect();
        refresh_all(); // ensure last frame was drawn properly

        new_action->set_enabled(true);
        load_action->set_enabled(true);
        save_action->set_enabled(true);
        next_action->set_enabled(true);
        canvas.set_sensitive(true);
    }
}

void window::next_generation()
{
    biotope->next();
    refresh_all();
}

void window::apply_speed()
{
    worker_naptime = std::chrono::milliseconds(static_cast<unsigned long>(1000L / speed_scale.get_value()));
}

void window::mouse_click(double x, double y)
{
    auto cx = static_cast<unsigned int>((x * biotope->width()) / (canvas.get_width()));
    auto cy = static_cast<unsigned int>((y * biotope->height()) / (canvas.get_height()));

    if (biotope->is_alive(cx, cy))
        biotope->kill(cx, cy);
    else
        biotope->create(cx, cy);

    refresh_cell(cx, cy);
    update();
}
