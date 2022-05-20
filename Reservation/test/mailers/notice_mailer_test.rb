require "test_helper"

class NoticeMailerTest < ActionMailer::TestCase
  test "register" do
    mail = NoticeMailer.register
    assert_equal "Register", mail.subject
    assert_equal ["to@example.org"], mail.to
    assert_equal ["from@example.com"], mail.from
    assert_match "Hi", mail.body.encoded
  end

end
