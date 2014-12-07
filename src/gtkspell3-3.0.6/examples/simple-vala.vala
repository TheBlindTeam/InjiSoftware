public class Window : Gtk.Window {
  private Gtk.Label label = new Gtk.Label (null);

  construct {
    this.set_default_size (400, 300);
    this.title = "Simple GtkSpell Demonstration";

    Gtk.Box box = new Gtk.Box (Gtk.Orientation.VERTICAL, 5);
    this.add (box);
    box.pack_start (label, false, false);

    var scroll = new Gtk.ScrolledWindow (null, null);
    box.pack_start (scroll);
    scroll.set_policy (Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC);
    scroll.shadow_type = Gtk.ShadowType.IN;

    var view = new Gtk.TextView ();
    scroll.add (view);
    view.wrap_mode = Gtk.WrapMode.WORD;

    var spell = new GtkSpell.Checker ();
    spell.attach (view);

    try {
      this.label.label =
        "Type some text into the text box.\n" +
        "Try misspelling some words.  Then right-click on them.";
    } catch ( GLib.Error e ) {
      this.label.label =
        "GtkSpell was unable to initialize.\n" +
        e.message;
    }

    box.show_all ();
  }
}

private static int main (string[] args) {
  Gtk.init (ref args);

  var win = new Window ();
  win.show ();
  win.destroy.connect (Gtk.main_quit);

  Gtk.main ();

  return 0;
}
