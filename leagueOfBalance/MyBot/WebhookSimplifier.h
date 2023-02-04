// +=========+
// | READ ME |
// +=========+
/*
* To enable the singleton, #define GLOBAL_WEBHOOKSIMPLIFIER before including or define it here to enable it everywhere
* To enable message logs, #define WHS_ENABLE_MESSAGE_LOGS before including or define it in the cpp file to enable it everywhere
*/

#pragma once
#define GLOBAL_WEBHOOKSIMPLIFIER

#ifndef WEBHOOKSIMPLIFIER
#define WEBHOOKSIMPLIFIER WebhookSimplifier
#include <string>

namespace dpp
{
	class cluster;
	class webhook;
}

class WebhookSimplifier final
{
public:
	/*
	* Don't forget to initialise me later with ChangeURL
	*/
	WebhookSimplifier();
	/*
	* \param webhookUrl: url of the webhook which you can find on discord
	*/
	WebhookSimplifier(const std::string& webhookUrl);
	~WebhookSimplifier();

	WebhookSimplifier(const WebhookSimplifier& whs) = delete;
	WebhookSimplifier(WebhookSimplifier&& whs) = delete;
	WebhookSimplifier& operator=(const WebhookSimplifier& whs) = delete;
	WebhookSimplifier& operator=(WebhookSimplifier&& whs) = delete;

	/*
	* \param webhook_url: url of the webhook which you can find on discord
	*/
	void ChangeURL(const std::string& webhook_url);
	/*
	* Tip: use Discord Developer mode to retrieve messageIds
	* \param messageId: Id of the message you want to edit when callign updateMessage.
	*/
	void ChangeMessageToEdit(uint64_t messageId);

	void CreateMessage(const std::string& message);
	/*
	* Set the Id first with ChangeMessageToEdit
	*/
	void UpdateMessage(const std::string& message);

	/*
	* Gets the content of the message with Id that is stored. See ChangeMessageToEdit
	*/
	const std::string& GetCurrentMessageContent() const;
private:
	static dpp::cluster* c_Bot;
	static size_t c_AmountActiveWebHookSimplifiers;

	dpp::webhook* m_pWebhook;
	uint64_t m_MessageId;
};

#ifdef GLOBAL_WEBHOOKSIMPLIFIER
#define WBS_MASTER gWebhookSimplifier::GetInstance()
class gWebhookSimplifier final
{
private:
	gWebhookSimplifier();

	static gWebhookSimplifier* c_pWbsMaster;

	WebhookSimplifier* m_pWebhookSimplifier;

public:
	static gWebhookSimplifier* GetInstance();
	static void ReleaseResources();

	~gWebhookSimplifier();

	gWebhookSimplifier(const gWebhookSimplifier& whs) = delete;
	gWebhookSimplifier(gWebhookSimplifier&& whs) = delete;
	gWebhookSimplifier& operator=(const gWebhookSimplifier& whs) = delete;
	gWebhookSimplifier& operator=(gWebhookSimplifier&& whs) = delete;

	void ChangeURL(const std::string& webhook_url);
	void ChangeMessageToEdit(uint64_t messageId);

	void CreateMessage(const std::string& message);
	void UpdateMessage(const std::string& message);

	const std::string& GetCurrentMessageContent() const;
};
#endif GLOBAL_WEBHOOKSIMPLIFIER

#endif WEBHOOKSIMPLIFIER