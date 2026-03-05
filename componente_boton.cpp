#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class Shape { Ellipse = 1, Rectangle = 2, Hexagon = 3 };

std::string toString(Shape shape) {
    switch (shape) {
    case Shape::Ellipse:
        return "Elipse";
    case Shape::Rectangle:
        return "Rectangulo";
    case Shape::Hexagon:
        return "Hexagono";
    default:
        return "Desconocida";
    }
}

class CommandButton {
  private:
    std::string id_;
    std::string text_;
    Shape shape_;
    int width_;
    int height_;
    bool enabled_;
    bool visible_;

    std::function<void(const CommandButton &)> onClick_;
    std::function<void(const CommandButton &)> onMouseEnter_;
    std::function<void(const CommandButton &)> onMouseLeave_;

  public:
    CommandButton(std::string id, std::string text, Shape shape, int width, int height)
        : id_(std::move(id)), text_(std::move(text)), shape_(shape), width_(width), height_(height),
          enabled_(true), visible_(true) {
        if (width_ <= 0 || height_ <= 0) {
            throw std::invalid_argument("Las dimensiones deben ser mayores a cero.");
        }
    }

    // Propiedades
    const std::string &id() const { return id_; }
    const std::string &text() const { return text_; }
    Shape shape() const { return shape_; }
    int width() const { return width_; }
    int height() const { return height_; }
    bool enabled() const { return enabled_; }
    bool visible() const { return visible_; }

    void setText(const std::string &value) { text_ = value; }
    void setShape(Shape value) { shape_ = value; }
    void setEnabled(bool value) { enabled_ = value; }
    void setVisible(bool value) { visible_ = value; }

    void resize(int newWidth, int newHeight) {
        if (newWidth <= 0 || newHeight <= 0) {
            throw std::invalid_argument("Las dimensiones deben ser mayores a cero.");
        }
        width_ = newWidth;
        height_ = newHeight;
    }

    // Métodos de "componente"
    void draw() const {
        if (!visible_) {
            std::cout << "[" << id_ << "] oculto\n";
            return;
        }

        std::cout << "\n+--------------------------------------+\n";
        std::cout << "| COMPONENTE BOTON DE COMANDO          |\n";
        std::cout << "+--------------------------------------+\n";
        std::cout << " Id        : " << id_ << "\n";
        std::cout << " Texto     : " << text_ << "\n";
        std::cout << " Forma     : " << toString(shape_) << "\n";
        std::cout << " Tamaño    : " << width_ << " x " << height_ << "\n";
        std::cout << " Habilitado: " << (enabled_ ? "Si" : "No") << "\n";
        std::cout << " Visible   : " << (visible_ ? "Si" : "No") << "\n";
        std::cout << "+--------------------------------------+\n";
    }

    void click() const {
        if (!visible_) {
            std::cout << "No se puede hacer click: el boton esta oculto.\n";
            return;
        }
        if (!enabled_) {
            std::cout << "No se puede hacer click: el boton esta deshabilitado.\n";
            return;
        }

        std::cout << "[Evento] Click detectado en el boton '" << text_ << "'.\n";
        if (onClick_) {
            onClick_(*this);
        }
    }

    void mouseEnter() const {
        std::cout << "[Evento] MouseEnter en '" << text_ << "'.\n";
        if (onMouseEnter_) {
            onMouseEnter_(*this);
        }
    }

    void mouseLeave() const {
        std::cout << "[Evento] MouseLeave en '" << text_ << "'.\n";
        if (onMouseLeave_) {
            onMouseLeave_(*this);
        }
    }

    // Registro de eventos
    void onClick(std::function<void(const CommandButton &)> callback) { onClick_ = std::move(callback); }

    void onMouseEnter(std::function<void(const CommandButton &)> callback) {
        onMouseEnter_ = std::move(callback);
    }

    void onMouseLeave(std::function<void(const CommandButton &)> callback) {
        onMouseLeave_ = std::move(callback);
    }
};

Shape askShape() {
    int option = 0;
    std::cout << "Seleccione la forma del boton:\n";
    std::cout << " 1) Elipse\n";
    std::cout << " 2) Rectangulo\n";
    std::cout << " 3) Hexagono\n";
    std::cout << "Opcion: ";
    std::cin >> option;

    switch (option) {
    case 1:
        return Shape::Ellipse;
    case 2:
        return Shape::Rectangle;
    case 3:
        return Shape::Hexagon;
    default:
        std::cout << "Opcion invalida. Se utilizara Rectangulo por defecto.\n";
        return Shape::Rectangle;
    }
}

void menu() {
    std::cout << "\n===== SIMULADOR DE COMPONENTE BOTON =====\n";
    std::cout << "1) Dibujar componente\n";
    std::cout << "2) Cambiar forma\n";
    std::cout << "3) Cambiar texto\n";
    std::cout << "4) Habilitar/Deshabilitar\n";
    std::cout << "5) Mostrar/Ocultar\n";
    std::cout << "6) Simular MouseEnter\n";
    std::cout << "7) Simular MouseLeave\n";
    std::cout << "8) Simular Click\n";
    std::cout << "9) Redimensionar\n";
    std::cout << "0) Salir\n";
    std::cout << "Seleccione una opcion: ";
}

int main() {
    std::cout << "Actividad 3 - Componente Propio (C++)\n";

    CommandButton button("btnEnviar", "Enviar", Shape::Rectangle, 140, 45);

    // Eventos personalizados
    button.onClick([](const CommandButton &b) {
        std::cout << " -> Accion de negocio: se ejecuto el comando asociado a " << b.id() << ".\n";
    });

    button.onMouseEnter([](const CommandButton &b) {
        std::cout << " -> Tooltip: Boton '" << b.text() << "' listo para interactuar.\n";
    });

    button.onMouseLeave([](const CommandButton &b) {
        std::cout << " -> El cursor salio de '" << b.text() << "'.\n";
    });

    int option = -1;
    while (option != 0) {
        menu();
        std::cin >> option;

        switch (option) {
        case 1:
            button.draw();
            break;
        case 2:
            button.setShape(askShape());
            std::cout << "Forma actualizada.\n";
            break;
        case 3: {
            std::cout << "Nuevo texto: ";
            std::cin.ignore();
            std::string text;
            std::getline(std::cin, text);
            button.setText(text);
            std::cout << "Texto actualizado.\n";
            break;
        }
        case 4:
            button.setEnabled(!button.enabled());
            std::cout << "Estado de habilitacion: " << (button.enabled() ? "Habilitado" : "Deshabilitado")
                      << "\n";
            break;
        case 5:
            button.setVisible(!button.visible());
            std::cout << "Estado de visibilidad: " << (button.visible() ? "Visible" : "Oculto") << "\n";
            break;
        case 6:
            button.mouseEnter();
            break;
        case 7:
            button.mouseLeave();
            break;
        case 8:
            button.click();
            break;
        case 9: {
            int w, h;
            std::cout << "Nuevo ancho: ";
            std::cin >> w;
            std::cout << "Nuevo alto: ";
            std::cin >> h;
            try {
                button.resize(w, h);
                std::cout << "Tamaño actualizado.\n";
            } catch (const std::exception &e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 0:
            std::cout << "Fin del programa.\n";
            break;
        default:
            std::cout << "Opcion no valida.\n";
            break;
        }
    }

    return 0;
}
