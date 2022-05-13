require "test_helper"

class EntrysControllerTest < ActionDispatch::IntegrationTest
  test "should get new" do
    get entrys_new_url
    assert_response :success
  end

  test "should get create" do
    get entrys_create_url
    assert_response :success
  end

  test "should get destroy" do
    get entrys_destroy_url
    assert_response :success
  end

  test "should get index" do
    get entrys_index_url
    assert_response :success
  end

  test "should get confirm" do
    get entrys_confirm_url
    assert_response :success
  end
end
