#include "chatbot.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

Chatbot::Chatbot(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    // No extra setup needed
}

void Chatbot::sendMessage(const QString &question)
{
    if (question.trimmed().isEmpty()) {
        emit errorOccurred("Le message est vide.");
        return;
    }

    // Prepare request to local Ollama server
    QNetworkRequest request(QUrl("http://127.0.0.1:11434/api/generate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["model"] = "gemma:2b";  // ✅ Using Gemma 2B
    json["prompt"] = "You are SDS.AI, the official AI assistant of Smart Content Creation Store in Tunisia. "
                     "You are a certified expert in smart content creation.\n\n"
                     "✅ Answer ONLY from the database. Be clear, professional, and helpful.\n"
                     "❌ Never answer off-topic questions.\n\n"
                     "Question: " + question;
    json["stream"] = false;
    json["options"] = QJsonObject{
        {"num_ctx", 1024},
        {"temperature", 1.0}
    };

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReplyFinished(reply);
    });
}

void Chatbot::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Erreur réseau : " + reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (!doc.isObject()) {
        emit errorOccurred("Réponse invalide (pas de JSON).");
        return;
    }

    QString response = doc.object()["response"].toString().trimmed();
    if (response.isEmpty()) {
        emit errorOccurred("Réponse vide de Gemma.");
        return;
    }

    emit responseReceived(response);
}
