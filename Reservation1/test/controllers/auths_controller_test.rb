require "test_helper"

class AuthsControllerTest < ActionDispatch::IntegrationTest
  test "should get new" do
    get auths_new_url
    assert_response :success
  end
end
