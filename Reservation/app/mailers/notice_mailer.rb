class NoticeMailer < ApplicationMailer

  # Subject can be set in your I18n file at config/locales/en.yml
  # with the following lookup:
  #
  #   en.notice_mailer.register.subject
  #
  def register(entry)
    @message = "#{entry.user_name}さん，申し込みありがとうございます．"
    @url = "http://www.example.com"
    mail to: entry.user_email,
        cc: "koki.victory.630@keio.jp",
        subject: "会議室予約確定のご案内"
  end
end
