#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebSocket>
#include <QTimer>
#include <QJsonObject>
#include <QStyle>

class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QSlider;
class QCheckBox;
class QPlainTextEdit;
class QLineSeries;
class QValueAxis;

// Una sola ventana: conexión, cruceta de movimiento, servo y gráfica de distancia.
// La interfaz se arma en código (sin .ui) para que se adapte al tamaño del teléfono.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // Conexión
    void alPresionarConectar();
    void alConectar();
    void alDesconectar();
    void alRecibirMensaje(const QString &texto);
    void alOcurrirError(QAbstractSocket::SocketError error);
    // Movimiento: mientras el botón esté presionado se repite la orden
    void repetirMovimiento();
    // Servo y distancia
    void alMoverServo(int angulo);
    void enviarServoPendiente();
    void alPresionarMedir();
    void alPresionarBarrido();
    void siguientePasoBarrido();

private:
    void construirInterfaz();
    QPushButton *botonMovimiento(QStyle::StandardPixmap icono, const QString &accion);
    void enviar(const QJsonObject &obj);
    void enviarMover(const QString &accion, int ms);
    void agregarDistancia(double cm);
    void registrar(const QString &linea);

    QWebSocket m_socket;
    QTimer m_latido;          // {"tipo":"latido"} cada 1 s (la ESP32 responde latido_ok)
    QTimer m_repetir;         // reenvía "mover" mientras se mantiene presionado un botón
    QTimer m_servo;           // limita el envío del servo a uno cada 60 ms
    QTimer m_medirAuto;       // lectura periódica de distancia
    QTimer m_barrido;         // pasos del barrido con "scan"

    QString m_accionActual;   // acción repetida por m_repetir
    int m_anguloPendiente = -1;
    int m_anguloBarrido = 0;
    int m_muestra = 0;        // eje X de la gráfica (número de muestra)

    // Widgets
    QLineEdit *m_ip = nullptr;
    QSpinBox *m_puerto = nullptr;
    QPushButton *m_conectar = nullptr;
    QLabel *m_estado = nullptr;
    QSlider *m_slider = nullptr;
    QLabel *m_angulo = nullptr;
    QLabel *m_distancia = nullptr;
    QCheckBox *m_auto = nullptr;
    QPlainTextEdit *m_registro = nullptr;
    QLineSeries *m_serie = nullptr;
    QValueAxis *m_ejeX = nullptr;
};

#endif // MAINWINDOW_H
