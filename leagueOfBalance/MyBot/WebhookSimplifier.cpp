#include "WebhookSimplifier.h"
#include <dpp/dpp.h>
#include <dpp/webhook.h>

#ifdef WHS_ENABLE_MESSAGE_LOGS
#include <iostream>
#endif

dpp::cluster* WebhookSimplifier::c_Bot{ nullptr };
size_t WebhookSimplifier::c_AmountActiveWebHookSimplifiers{ 0 };

WebhookSimplifier::WebhookSimplifier()
	:m_pWebhook{ new dpp::webhook{} }
	,m_MessageId{ 0 }
{
	if (c_Bot == nullptr)
	{
		c_Bot = new dpp::cluster{""};
	}
	++c_AmountActiveWebHookSimplifiers;
}

WebhookSimplifier::WebhookSimplifier(const std::string& webhookUrl)
	:m_pWebhook{ new dpp::webhook{webhookUrl} }
	,m_MessageId{ 0 }
{
	if (c_Bot == nullptr)
	{
		c_Bot = new dpp::cluster{ "" };
	}
	++c_AmountActiveWebHookSimplifiers;
}

WebhookSimplifier::~WebhookSimplifier()
{
	--c_AmountActiveWebHookSimplifiers;
	if (c_AmountActiveWebHookSimplifiers == 0)
	{
		delete c_Bot;
		c_Bot = nullptr;
	}
}

void WebhookSimplifier::ChangeURL(const std::string& webhook_url)
{
	m_pWebhook->token = webhook_url.substr(webhook_url.find_last_of("/") + 1);
	try {
		m_pWebhook->id = std::stoull(webhook_url.substr(strlen("https://discord.com/api/webhooks/"), webhook_url.find_last_of("/")));
	}
	catch (const std::exception& e) {
		throw dpp::logic_exception(std::string("Failed to parse webhook URL: ") + e.what());
	}
}

void WebhookSimplifier::ChangeMessageToEdit(uint64_t messageId)
{
	m_MessageId = messageId;
}

void WebhookSimplifier::CreateMessage(const std::string& message)
{
	c_Bot->execute_webhook_sync(*m_pWebhook, dpp::message(message));
#ifdef WHS_ENABLE_MESSAGE_LOGS
	std::cout << "[WHS] New message:\n{\n\n" << message << "\n\n}\n";
#endif 
}

void WebhookSimplifier::UpdateMessage(const std::string& message)
{
	if (m_MessageId == 0) return;

	auto prevMsg = c_Bot->get_webhook_message_sync(*m_pWebhook, m_MessageId);

	prevMsg.content = message;

	c_Bot->edit_webhook_message_sync(*m_pWebhook, prevMsg);
#ifdef WHS_ENABLE_MESSAGE_LOGS
	std::cout << "[WHS] Updated message (ID:" << m_MessageId << "):\n{\n\n" << message << "\n\n}\n";
#endif
}

const std::string& WebhookSimplifier::GetCurrentMessageContent() const
{
	return (c_Bot->get_webhook_message_sync(*m_pWebhook, m_MessageId).content);
}

// ============================================================================================

#ifdef GLOBAL_WEBHOOKSIMPLIFIER

gWebhookSimplifier* gWebhookSimplifier::c_pWbsMaster{ nullptr };

gWebhookSimplifier* gWebhookSimplifier::GetInstance()
{
	if (c_pWbsMaster)
	{
		return c_pWbsMaster;
	}
	c_pWbsMaster = new gWebhookSimplifier();
	return c_pWbsMaster;
}

gWebhookSimplifier::gWebhookSimplifier()
	:m_pWebhookSimplifier{ new WebhookSimplifier{} }
{
}

void gWebhookSimplifier::ReleaseResources()
{
	if (c_pWbsMaster)
	{
		delete c_pWbsMaster;
		c_pWbsMaster = nullptr;
	}
}

gWebhookSimplifier::~gWebhookSimplifier()
{
	delete m_pWebhookSimplifier;
	m_pWebhookSimplifier = nullptr;
}

void gWebhookSimplifier::ChangeURL(const std::string& webhook_url)
{
	m_pWebhookSimplifier->ChangeURL(webhook_url);
}

void gWebhookSimplifier::ChangeMessageToEdit(uint64_t messageId)
{
	m_pWebhookSimplifier->ChangeMessageToEdit(messageId);
}

void gWebhookSimplifier::CreateMessage(const std::string& message)
{
	m_pWebhookSimplifier->CreateMessage(message);
}

void gWebhookSimplifier::UpdateMessage(const std::string& message)
{
	m_pWebhookSimplifier->UpdateMessage(message);
}

const std::string& gWebhookSimplifier::GetCurrentMessageContent() const
{
	return m_pWebhookSimplifier->GetCurrentMessageContent();
}

#endif // GLOBAL_WEBHOOKSIMPLIFIER