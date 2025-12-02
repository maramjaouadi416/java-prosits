#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Chatbot : public QObject
{
    Q_OBJECT

public:
    explicit Chatbot(QObject *parent = nullptr);

    // Send a question to Ollama (Gemma 2B) via HTTP
    void sendMessage(const QString &question);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &error);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
};

#endif // CHATBOT_H
