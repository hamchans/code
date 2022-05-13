class TopController < ApplicationController
#  USERS = {"admin_user" => "admin_password"}
#  before_action :authenticate

  def index
    # TODO: トップページです．（適切な挨拶文をビューに記載してください）
  end
  
  private
    def authenticate
      authenticate_or_request_with_http_digest do |username|
        USERS[username]
      end
    end
end
