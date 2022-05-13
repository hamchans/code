json.extract! room, :id, :name, :place, :number, :created_at, :updated_at
json.url room_url(room, format: :json)
