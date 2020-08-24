import gql from 'graphql-tag'

export const PRODUCERS_SUBSCRIPTION = gql`
  subscription {
    producer(order_by: { total_votes_percent: desc, owner: asc }) {
      id
      owner
      url
      ping
      total_votes
      bp_json
      total_votes_percent
      total_votes_eos
      total_rewards
      server_version_string
      head_block_producer
      head_block_time
      updated_at
    }
  }
`
